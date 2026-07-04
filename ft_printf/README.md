*This project has been created as part of the 42 curriculum by taegokim.*

# ft_printf

## Description

C 표준 라이브러리의 `printf` 함수를 부분적으로 재구현한 프로젝트입니다. 가변 인자 함수, 포맷 문자열 파싱, 저수준 출력 처리를 직접 구현하며 깊이 이해하는 것이 목표입니다.


지원하는 변환 지정자: (*보너스 파트는 진행하지 않음*)

| 지정자 | 설명 |
|--------|------|
| `%c` | 단일 문자 |
| `%s` | 문자열 (`NULL` → `(null)`) |
| `%p` | 포인터 주소, 16진수 + `0x` 접두사 (`NULL` → `(nil)`) |
| `%d` / `%i` | 부호 있는 10진 정수 |
| `%u` | 부호 없는 10진 정수 |
| `%x` / `%X` | 부호 없는 16진 정수 (소문자 / 대문자) |
| `%%` | `%` 문자 출력 |

반환값은 `printf`와 동일합니다: 출력한 문자 수, 오류 시 `-1`.

---

## Algorithm & Data Structure

### 형식지정자 출력 핸들러에 함수포인터 배열 사용

형식지정자가 한 글자인 점에서 착안하여 **256개 원소의 함수 포인터 배열**로, 지정자 문자의 ASCII 값을 인덱스로 사용합니다:

```c
typedef int		(*t_print_format)(va_list);

t_print_format pf[256];
pf['c'] = print_format_c;
pf['d'] = print_format_d;
...
```

`ft_printf`가 `%`를 만나면 다음 문자를 읽어 즉시 `pf[(unsigned char)c](args)`를 호출합니다. if-else 체인이나 switch 없이 **O(1) 동작**이 가능합니다.

### 정수 → 문자열 변환 (`nbr2base_str` 등)

임의 진수 변환을 2단계로 처리합니다:
1. `cnt_base_x`로 자릿수를 먼저 세어 정확한 크기의 버퍼를 할당합니다.
2. `"0123456789abcdef"[n % base]`로 오른쪽부터 채우고 나누어 내려갑니다.

이 방식으로 과잉 할당을 피하고, 문자열을 뒤집는 후처리도 필요 없습니다.

### 포인터[%p] 처리를 위한 별도 함수 사용 (`ptr2base_str` 등)

포인터를 정수로 변환할 때 `uintptr_t` (`<stdint.h>`) 타입으로 캐스팅합니다. `uintptr_t`는 플랫폼의 포인터 크기를 항상 담을 수 있도록 보장된 타입입니다. 기존의 `nbr2base_str`(`long long`)을 사용하면 부호 있는 타입이므로 높은 주소값이 음수로 해석되어 변환 결과가 틀릴 수 있습니다.

### 정적 라이브러리 구성

프로젝트는 `libftprintf.a` 하나를 생성합니다. `libft.a`를 복사한 뒤 `ft_printf`의 `.o` 파일들을 `ar rcs`로 추가하는 방식으로, 사용자는 단일 아카이브만 링크하면 됩니다.

---

## Instructions

### 빌드

```bash
make			# 프로젝트 빌드
make clean    	# 오브젝트 파일 제거
make fclean   	# 오브젝트 파일 + libftprintf.a 제거
make re			# fclean + all
```

프로젝트 루트에 `libftprintf.a`가 생성됩니다.

### 본인 프로젝트에서 링크하기

```bash
gcc your_file.c -L. -lftprintf
```

---

## Resources

- [printf(3) — Linux man page](https://man7.org/linux/man-pages/man3/printf.3.html)
- [stdarg.h — C 가변 인자 함수](https://en.cppreference.com/w/c/variadic)
- [uintptr_t — 포인터-정수 변환](https://en.cppreference.com/w/c/types/integer)
- [ar(1) — GNU Binutils 아카이브 툴](https://www.ibm.com/docs/ko/aix/7.2.0?topic=ar-command)
- 42 subject PDF: `ft_printf.pdf` 

### AI usage

이 프로젝트에서 Claude (Claude Code)를 다음 용도로 활용했습니다:

- **디버깅**: Makefile에서 `$(OBJS)`가 비어 라이브러리가 링크되지 않던 원인 파악.
- **설계 논의**: `%p` 전용 변환 함수를 별도로 만들지, `uintptr_t` 캐스팅으로 `nbr2base_str`을 재사용할지 트레이드오프 검토.
- **엣지 케이스 발굴**: `NULL` 포인터, `INT_MIN`, `UINT_MAX`, null 문자(`%c`에 0), 빈 문자열, 복합 지정자 조합 등 테스트 케이스 보강.

