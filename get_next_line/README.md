*This project has been created as part of the 42 curriculum by taegokim.*

# get_next_line

## Description

`get_next_line`은 파일 디스크립터에서 한 번에 한 줄씩 읽어 반환하는 C 함수입니다. 줄의 끝에 있는 개행 문자(`\n`)도 반환 문자열에 포함되며, 반복 호출을 통해 파일 전체를 한 줄씩 읽을 수 있습니다. 일반 파일과 표준 입력(stdin) 모두에서 올바르게 동작합니다.

**목표:** 아래 프로토타입의 함수를 구현합니다.

```c
char *get_next_line(int fd);
```

- 호출할 때마다 `fd`에서 다음 줄을 반환합니다.
- 파일을 모두 읽었거나 오류가 발생하면 `NULL`을 반환합니다.
- `\n`이 존재하면 반환 문자열에 포함됩니다. 파일의 마지막 줄에 개행이 없으면 그대로 반환됩니다.
- 한 번의 `read(2)` 호출로 읽는 바이트 수는 컴파일 타임 매크로 `BUFFER_SIZE`로 지정합니다.

프로젝트는 두 파일로 구성됩니다.

| 파일 | 역할 |
|---|---|
| `get_next_line.c` | 핵심 로직: `read_to_leftover`, `extract_line`, `trim_leftover`, `get_next_line` |
| `get_next_line_utils.c` | 보조 함수: `gnl_strlen`, `gnl_strchr`, `gnl_strjoin` |

---

## Algorithm

### 접근 방식: leftover 누적 방식

`read(2)` 시스템 콜은 파일 오프셋을 앞으로만 진행시키며 되돌릴 수 없습니다. 따라서 `\n`을 찾기 위해 읽은 데이터가 다음 줄까지 넘어갔다면, 그 초과분을 어딘가에 보관해야 합니다. 

이를 위해 `static char *leftover`를 사용하여 호출 간에 아직 반환되지 않은 데이터를 유지합니다. `leftover`는 `\n`을 발견할 때까지 `read` 결과를 `gnl_strjoin`으로 계속 이어붙인 힙 문자열입니다.

`get_next_line`이 호출될 때마다 다음 순서로 동작합니다.

```
┌─────────────────────────────────────────────────────┐
│  read_to_leftover                                   │
│    └── '\n' 발견될 때까지 read → gnl_strjoin 반복   │
│         ├── read 오류 → buf, leftover free 후 NULL  │
│         └── EOF → 현재 leftover 그대로 반환          │
├─────────────────────────────────────────────────────┤
│  extract_line                                       │
│    └── leftover[0..'\n'] 복사 → 반환할 line 생성    │
├─────────────────────────────────────────────────────┤
│  trim_leftover                                      │
│    └── '\n' 이후 남은 문자열 → 다음 호출용 leftover  │
│         └── '\n' 뒤가 비어 있으면 NULL 반환          │
└─────────────────────────────────────────────────────┘
line 반환  (EOF 또는 오류 시 NULL)
```

**핵심 조건:** 

`gnl_strjoin`은 내부에서 `s1`(기존 leftover)을 항상 `free`하고 새 문자열을 반환합니다. 따라서 `leftover` 포인터는 매 `read` 이후 갱신됩니다.

`buf`는 `BUFFER_SIZE`가 매우 큰 수로 들어올 수 있으므로 스택 오버플로를 방지하기 위해 힙에 동적 할당하며, `read_to_leftover` 내부에서만 쓰이는 임시 버퍼이므로 함수 종료 시 항상 해제됩니다.

---

## Instructions

### 컴파일

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

`BUFFER_SIZE`는 양의 정수라면 어떤 값이든 설정 가능합니다. `BUFFER_SIZE <= 0`이면 `get_next_line`과 `read_to_leftover` 모두 즉시 `NULL`을 반환합니다.

### 테스트

위 `main.c`와 `file.txt`를 프로젝트 루트에 직접 만든 뒤, 컴파일 명령어를 실행하면 됩니다.

### main.c 예시

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int     fd;
    char    *line;

	// fd = 0; // stdin test
    fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		printf("파일 열기 실패\n");
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### file.txt 예시

```
Hello world!
My name is Kim Tae Gon.
```

---

## Resources

### 문서 및 참고자료

- [`read(2)` — Linux man 페이지](https://man7.org/linux/man-pages/man2/read.2.html): 파일 디스크립터에서 바이트를 읽는 시스템 콜.
- [`open(2)` — Linux man 페이지](https://man7.org/linux/man-pages/man2/open.2.html): `get_next_line`에서 사용할 파일 열기.
- 42 Subject — get_next_line.pdf : 공식 프로젝트 명세서.

### AI 활용 내역

이 프로젝트에서 Claude code 를 다음 목적으로 활용하였습니다.

- **README 작성**: 이 문서의 구조 설계 및 내용 작성에 사용했습니다.
- **프로그램 아키텍처 구상**: 프로그램 동작 설계시 참고하였습니다.
- **norminette**: 함수 라인 수를 25줄 이하로 줄이는 리팩토링에 활용했습니다. 에러 경로에서 `return (free(buf), free(leftover), NULL)` 패턴을 사용하여 블록 없이 한 줄로 정리하는 방식을 적용했습니다.
- **Deepthought 오류 추적** : 기계 채점시 오류 메시지(`Read with a size different than BUFFER_SIZE`) 분석에 사용하였습니다.
- **빈 문자열 leftover 처리 및 댕글링 포인터 디버깅**: EOF 상황일 때, leftover가 빈 문자열이 되어 `get_next_line`에서 `leftover[0] == '\0'` 체크를 추가하고, `free` 후 반드시 `leftover = NULL`을 해야 static 변수의 댕글링 포인터를 방지할 수 있음을 확인했습니다.