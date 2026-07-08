*This project has been created as part of the 42 curriculum by taegokim.*

# pipex

## Table of Contents

- [Description](#description)
- [Instructions](#instructions)
- [Architecture](#architecture)
- [Key Design Decisions](#key-design-decisions)
- [Bugs Found & Fixes](#bugs-found--fixes)
- [Test Cases & Edge Cases](#test-cases--edge-cases)
- [Resources](#resources)

## Description

`pipex`는 bash의 파이프라인 문법인 다음 명령을 C로 재구현하는 42 과제입니다.

```bash
< infile cmd1 | cmd2 | ... | cmdN > outfile
```

즉 `infile`을 첫 커맨드의 표준 입력으로, 마지막 커맨드의 표준 출력을 `outfile`로 연결하고, 그 사이 커맨드들을 파이프로 이어서 실행하는 프로그램입니다. 보너스로 heredoc(`<<`) 입력도 지원합니다.

```bash
./pipex infile cmd1 cmd2 outfile
./pipex here_doc LIMITER cmd1 cmd2 outfile
```

목표는 단순히 "동작하는 프로그램"을 만드는 게 아니라, **에러 상황에서도 bash와 동일하게 동작하는 것**입니다 — 존재하지 않는 파일, 권한 없는 파일, 존재하지 않는 커맨드가 파이프라인 중간에 있어도 나머지 커맨드는 계속 실행되어야 하고, 최종 exit code는 bash 규칙(마지막 커맨드 기준)을 따라야 합니다.

## Instructions

### 빌드

```bash
make        # pipex 바이너리 생성
make re     # 클린 빌드
make clean  # 오브젝트 파일 정리
make fclean # 바이너리 + 오브젝트 + 서브모듈(ft_printf, libft) 정리
```

### 실행

```bash
# mandatory: 파일 입력
./pipex infile "cmd1 arg1" "cmd2 arg2" outfile

# bonus: heredoc 입력 (LIMITER를 입력하면 종료)
./pipex here_doc LIMITER "cmd1" "cmd2" outfile
```

커맨드는 3개 이상 이어도 됩니다 (`./pipex infile cmd1 cmd2 cmd3 cmd4 outfile`).

### 의존성

- 자체 구현한 `ft_printf`(`ft_printf/`), `libft`(`ft_printf/libft/`), `get_next_line`(`get_next_line/`)을 서브모듈처럼 포함하고 있으며 `Makefile`이 자동으로 빌드합니다.
- 외부 라이브러리 의존성 없음 (POSIX 시스템 콜만 사용: `open`, `close`, `read`, `write`, `access`, `dup`/`dup2`, `execve`, `exit`, `fork`, `pipe`, `unlink`, `wait`/`waitpid`).

## Architecture

프로젝트는 "구조체 + 함수 포인터"로 C에서 객체지향 스타일을 흉내내는 방식으로 짜여 있습니다. 각 모듈은 `_init` 함수로 초기화되고, 자기 자신을 대상으로 동작하는 함수 포인터(`run`, `destroy` 등)를 필드로 가집니다.

| 모듈 | 역할 |
|---|---|
| `parser` | argv를 파싱해 `t_parsed`(입력 타입, infile/limiter, outfile, 커맨드 목록)로 변환 |
| `reader` | 첫 커맨드의 표준 입력이 될 fd를 준비 (FILE: `open()`, HEREDOC: 임시 파일에 수집) |
| `writer` | 마지막 커맨드의 표준 출력이 될 outfile fd를 준비 |
| `pipe_mgr` | 커맨드 사이를 잇는 파이프들을 생성/보관/일괄 정리 |
| `cmd` / `cmd_mgr` | 커맨드 하나하나를 fork + execve로 실행, PATH에서 실행 파일 경로 탐색 |
| `app` | 위 모듈들을 초기화 순서대로 엮고 실행/정리하는 최상위 오케스트레이터 |

### 데이터 흐름

```
reader.fd_in ─┐
              ├─▶ cmd[0] ─(pipe)─▶ cmd[1] ─(pipe)─▶ ... ─▶ cmd[N-1] ─┐
              │                                                       ├─▶ writer.fd_out
   (dup2)     └───────────────────────────────────────────────(dup2)─┘
```

`reader`/`writer`는 프로세스를 만들지 않고 **fd만 준비**하며, 그 fd는 `pipe_mgr`가 관리하는 파이프 배열의 양 끝 슬롯에 그대로 꽂혀서 `cmd[0]`/`cmd[N-1]`이 `dup2`로 직접 사용합니다. (아래 "Key Design Decisions" 참고)

## Key Design Decisions

### 1. 추상 팩토리 대신 디스패치 테이블

초기 설계는 입력 타입(FILE/HEREDOC)에 따라 동작을 바꾸기 위해 "리더 팩토리" 객체(`create_fd_in`, `create_read` 메서드를 가진 별도 구조체)를 만드는 방향이었습니다. 하지만:

- 타입은 파싱 시점에 이미 확정되고, 팩토리 객체는 생성 직후 바로 소비되어 재사용되지 않음
- 여러 호출부가 팩토리 객체 자체를 다형적으로 주고받지 않음 (Abstract Factory 패턴이 가치를 가지는 조건이 아님)

이런 경우엔 GoF 패턴보다 **`enum` 인덱스로 함수 포인터를 바로 찾는 정적 디스패치 테이블**이 더 적합합니다.

```c
static int (*const create_fd_in[TYPE_COUNT])(t_parsed *parsed) = {
	[TYPE_FILE] = create_file_fd_in,
	[TYPE_HEREDOC] = create_heredoc_fd_in,
};
```

### 2. reader/writer는 별도 프로세스를 만들지 않는다

처음엔 `reader`/`writer`도 각자 fork해서 `read()`/`write()` 루프로 데이터를 파이프에 중계하는 방식이었습니다. 이 방식은 두 가지 문제가 있었습니다.

- FILE 입력 하나를 옮기려고 불필요한 프로세스를 하나 더 fork함
- (아래 SIGPIPE 버그 참고) 중계 프로세스가 파이프에 `write()`하는 도중 다운스트림 커맨드가 stdin을 안 읽고 먼저 종료해버리면 크래시

그래서 최종적으로는 **`reader_init`/`writer_init`이 fd만 준비**해두고, 그 fd를 `cmd[0]`/마지막 `cmd`가 **직접 `dup2`** 해서 쓰는 구조로 바꿨습니다. 중계 프로세스 자체가 없으니 이 클래스의 버그가 원천적으로 발생하지 않습니다.

이때 `pipe_mgr`의 파이프 배열 인덱싱(`pipes[idx][0]`/`pipes[idx+1][1]`)은 그대로 두고, 양 끝 슬롯의 "실제로 쓰이는 반쪽"만 진짜 `pipe()` 대신 reader/writer가 준비한 fd로 채워 넣었습니다. 안 쓰는 반쪽은 `-1` 센티널로 표시해 `close_all_pipes`가 건너뛰게 했습니다. 덕분에 `cmd_init`은 커맨드 위치와 무관하게 완전히 동일한 방식으로 fd를 조회하고, "양 끝 커맨드만 특별 취급"하는 분기가 코드에 전혀 생기지 않습니다.

### 3. heredoc은 임시 파일을 거친다

heredoc 입력을 파이프에 실시간으로 스트리밍하면, 파이프라인의 첫 커맨드가 `ls`처럼 stdin을 안 읽고 즉시 종료하는 경우 SIGPIPE로 죽습니다 (아래 버그 항목 참고). 그래서 `get_next_line`으로 리미터까지 전부 읽어 **일반 파일**에 다 쓴 뒤, 그 파일을 다시 읽기 전용으로 열어서 `fd_in`으로 반환합니다. 일반 파일에 대한 `write()`는 읽는 쪽의 유무와 무관하게 절대 SIGPIPE가 나지 않습니다.

### 4. 에러가 나도 파이프라인은 멈추지 않는다

bash는 `< nofile cmd1 | cmd2` 같은 상황에서도 `cmd2`를 정상 실행합니다. 이를 재현하기 위해 일관된 원칙을 세웠습니다: **개별 자원 준비가 실패해도 전체 파이프라인 실행 자체를 막지 않는다.**

- infile을 못 열면 → 에러 출력 후 `/dev/null`로 대체, 파이프라인은 계속 진행
- outfile을 못 열면 → 에러 출력 후 `/dev/null`로 대체 (마지막 커맨드 출력만 버려짐)
- 커맨드를 PATH에서 못 찾으면 → `cmd_init` 단계에서 파이프라인을 죽이지 않고, 그 커맨드의 `execve()`가 자식 프로세스 안에서 실패하도록 그대로 진행 (다른 커맨드는 영향 없음)

## Bugs Found & Fixes

개발 중 리뷰와 실제 실행 테스트(valgrind 포함)로 발견한 문제들과 수정 방향입니다.

| 증상 / 발견 경위 | 원인 | 수정 |
|---|---|---|
| 컴파일 자체가 안 됨 (`reader_factory.c`) | `t_reader_factory` 구조체에 없는 `fd_in` 필드 접근, `this->` 누락, 미완성 대입문 | 팩토리 구조를 걷어내고 디스패치 테이블로 재작성 |
| `create_read` 함수 포인터 타입 불일치 | out-parameter로 함수 포인터를 넘기려면 `t_status (**)(t_reader*)`가 필요한데 시그니처가 제각각이었음 | 애초에 "함수 포인터를 만들어주는 함수"라는 설계를 폐기 |
| `wc -l` 등 파이프라인 결과가 항상 `0` / 파일 내용이 터미널에 그대로 출력됨 | `app_run_impl`에서 `cmd_mgr.run()`(커맨드 fork + 파이프 close)이 `reader.read()`(reader fork)보다 먼저 실행되어, reader가 fork될 때는 이미 부모가 쓰기용 파이프 fd를 닫아버린 뒤라 `dup2`가 조용히 실패 | fork 순서를 "전부 fork 후에 정리"로 통일 (아래 5번 항목과 함께 최종적으로는 reader/writer가 fork 자체를 안 하는 구조로 대체) |
| `double free or corruption` 크래시 | `parser.c`의 `parser_destroy`가 `free(this)` 호출 — `this`는 `main()`의 스택 변수 `t_app app` 안에 있는 필드라 힙 포인터가 아님. parser는 애초에 별도로 힙에 소유한 게 없음 | `free(this)` 제거, no-op destroy로 변경 |
| `cmd_mgr`의 자식 프로세스 destroy가 항상 스킵됨 | `if (!&this->cmds[i])` — 배열 원소의 주소는 절대 NULL이 아니라 조건이 항상 거짓 | `if (this->cmds[i].destroy)`로 수정 (calloc 초기화로 미생성 cmd는 자연히 NULL이라 안전하게 스킵됨) |
| `free_split(NULL)` 크래시 가능성 | `cmd_init`이 `ft_split` 단계에서 실패하면 `argv`가 NULL인 채로 destroy가 호출될 수 있음 | `free_split`에 NULL 가드 추가 |
| outfile을 못 열면 커맨드가 하나도 안 실행됨 | `writer_init`이 `open()` 실패 시 그대로 `FAIL` 반환 → `app_init` 실패 → `app_run_impl` 자체가 호출 안 됨 | reader와 동일하게 `/dev/null` 폴백, `writer_init`은 실패시키지 않음 |
| 존재하지 않는 커맨드가 있으면 파이프라인 전체가 안 돎 | `cmd_init`이 `create_cmd_path` 실패를 그 자리에서 `FAIL`로 전파 → 다른 커맨드까지 전부 fork 전에 중단 | 경로를 못 찾아도 `cmd_init`은 성공 처리하고, `argv[0]`을 그대로 넘겨 `execve()`가 자식 프로세스 안에서 실패하도록 함(127 종료) — 다른 커맨드는 정상 진행 |
| pipex 자신의 exit code가 항상 0 | 커맨드들의 종료 상태를 `waitpid(pid, NULL, 0)`로 버림, `main()`도 무조건 `return 0` | `cmd_mgr`에서 마지막 커맨드의 상태를 `WIFEXITED`/`WEXITSTATUS`/`WIFSIGNALED`로 계산해 보관, `main()`이 이를 반환 |
| heredoc + `ls`(stdin 안 읽는 커맨드) 조합에서 SIGPIPE로 프로세스 죽음 (valgrind로 발견) | heredoc reader 프로세스가 사용자가 타이핑하는 동안 실시간으로 파이프에 `write()`하는데, `ls`가 stdin을 안 읽고 즉시 종료해 파이프 읽기 쪽이 닫힘. `signal()`/`sigaction()`은 pipex 허용 함수 목록에 없어 SIGPIPE를 무시할 수 없음 | heredoc을 임시 파일에 전부 수집한 뒤 그 파일을 열어 쓰는 방식으로 변경, 나아가 reader/writer의 파이프 중계 프로세스 자체를 없애고 `dup2` 직결 구조로 전환 (Key Design Decisions 3, 2번 참고) |
| `Makefile`의 `SRCS`에 `reader.c` 등 실제 사용 파일 누락 | `app.c`가 `reader_init`/`writer_init`을 호출하는데 정작 그 구현 파일들이 빌드 대상에 없었음 | `SRCS`에 누락 파일 추가, `make re`로 전체 빌드 검증 |

## Test Cases & Edge Cases

### 정상 동작

```bash
$ ./pipex infile "cat" "wc -l" outfile
$ cat outfile        # infile의 줄 수

$ echo -e "a\nb\nEND" | ./pipex here_doc END "cat" "wc -l" outfile
$ cat outfile        # 2
```

### bash와 동일해야 하는 에러 상황

| 상황 | 명령 예시 | 기대 동작 |
|---|---|---|
| infile 없음/권한 없음 | `./pipex nofile cmd1 cmd2 outfile` | stderr에 에러, cmd1은 EOF로 실행(빈 입력), cmd2는 정상 실행, exit code는 cmd2 기준 |
| outfile 생성 불가 | `./pipex infile cmd1 cmd2 /no_permission/out` | stderr에 에러, 커맨드들은 정상 실행되지만 마지막 출력은 버려짐, exit code는 cmd2 기준 |
| 중간 커맨드가 존재하지 않음 | `./pipex infile nonexistent cmd2 outfile` | `command not found`, 해당 커맨드만 exit 127, 나머지 커맨드는 정상 실행 |
| 마지막 커맨드가 존재하지 않음 | `./pipex infile cmd1 nonexistent outfile` | `command not found`, pipex 전체 exit code가 127 |
| 파이프라인 exit code 전파 | `./pipex infile cat false outfile` | `$?`가 bash의 `cat infile \| false`와 동일 (1) |
| 명시적 경로로 준 커맨드가 없음 | `./pipex infile ./nonexistent cmd2 outfile` | PATH 탐색이 아니라 `access()` 결과 그대로라 `No such file or directory` (bash와 동일하게 `command not found`와 문구가 다름) |

### 엣지 케이스

- **다운스트림 커맨드가 stdin을 안 읽고 즉시 종료** (`./pipex infile ls "wc -l" outfile`, heredoc의 `ls`도 동일): 예전엔 SIGPIPE로 내부 프로세스가 죽었으나, dup2 직결 구조로 바뀐 뒤에는 커맨드 자신이 direct fd를 갖고 있어 문제되지 않음
- **heredoc 리미터와 정확히 일치해야 종료**: 리미터를 포함하는 줄(`"ENDX"`)이 실제 리미터(`"END"`)로 오인되지 않도록 길이까지 비교 (`is_limiter`)
- **heredoc 입력 중 Ctrl+D(EOF)**: 리미터 없이 EOF를 받아도 크래시 없이 그때까지 받은 내용으로 종료
- **커맨드 3개 이상 체이닝**: `./pipex infile cmd1 cmd2 cmd3 outfile` — 파이프 개수가 `command_num - 1`이 아니라 `command_num + 1`(양 끝 포함) 구조라 커맨드 개수가 늘어도 동일한 인덱싱 로직으로 처리됨
- **`cmd_init`이 중간에 실패(OOM 등)해도 이후 정리 단계에서 크래시 없음**: `calloc`으로 0-초기화된 미생성 cmd의 `.destroy`/`.pid`를 각각 NULL 체크·`pid <= 0` 체크로 걸러 잘못된 함수 포인터 호출이나 엉뚱한 프로세스 reap을 방지

## Resources

### 참고 자료

- [Advanced Programming in the UNIX Environment (Stevens & Rago)](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739) — `fork`/`exec`/`pipe`/시그널 전반
- `man 2 pipe`, `man 2 dup2`, `man 2 execve`, `man 2 fork`, `man 2 wait`, `man 2 waitpid` — 사용한 시스템 콜 공식 문서
- [Bash Reference Manual — Pipelines](https://www.gnu.org/software/bash/manual/bash.html#Pipelines) — 파이프라인의 exit code 규칙, 에러 처리 기준
- 42 `pipex`/`minishell` 관련 커뮤니티 자료 (heredoc을 임시 파일로 처리하는 관례 등)

### AI 활용 내역

이 프로젝트는 Claude Code(Anthropic)를 다음 용도로 활용했습니다.

- **설계 리뷰**: 초기 "리더 팩토리" 구조에 대해 GoF 패턴(Abstract Factory/Strategy)과의 적합성을 검토하고, 더 단순한 디스패치 테이블 구조로 리팩토링하는 과정 전반
- **코드 작성**: `create_cmd_path`(PATH 탐색), `reader`/`writer` 모듈의 fd 준비 로직, heredoc 임시 파일 처리, `cmd_mgr`의 exit code 계산 로직 등 구현
- **버그 헌팅 및 근본 원인 분석**: 컴파일 에러, fork 순서로 인한 파이프 fd 오사용, `double free or corruption`(스택 포인터를 `free()`한 버그), `cmd_mgr`의 죽은 코드(`if (!&this->cmds[i])`) 등을 실제 실행/valgrind 결과를 근거로 진단
- **bash 동작 재현 검증**: 에러 상황별로 실제 `bash`와 `pipex`의 exit code·출력을 나란히 실행/비교해 명세와 구현이 일치하는지 확인
- 최종 코드 작성과 설계 결정은 42 규칙(허용 함수 목록 등)에 맞는지 직접 검토·승인하며 진행했습니다.
