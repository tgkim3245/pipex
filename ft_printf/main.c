/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 16:17:32 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include "ft_printf.h"

/*
make
gcc -Wall -Werror -Wextra main.c -L. -lftprintf
*/

#define TEST(fmt, ...) do { \
	int a = ft_printf(fmt, ##__VA_ARGS__); \
	printf("\n"); \
	int b = printf(fmt, ##__VA_ARGS__); \
	printf("\n"); \
	printf("ft_printf ret: %d | printf ret: %d | %s\n\n", a, b, a == b ? "OK" : "FAIL"); \
} while (0)

static void	section(const char *title)
{
	printf("=== %s ===\n", title);
}

int	main(void)
{
	int		num;
	char	*s;
	void	*null_ptr;

	num = 42;
	s = "hello";
	null_ptr = NULL;

	section("%c");
	TEST("%c", 'A');
	TEST("%c", 'a');
	TEST("%c", '0');
	TEST("%c", ' ');
	TEST("%c", '\n');
	TEST("%c", '\t');
	TEST("%c", 0);          // null character

	section("%s");
	TEST("%s", "hello world");
	TEST("%s", "");
	TEST("%s", NULL);       // (null)
	TEST("%s", "a");
	TEST("%s", "한글");     // multibyte (byte count만 비교)

	section("%p");
	TEST("%p", &num);
	TEST("%p", s);
	TEST("%p", null_ptr);   // (nil)
	TEST("%p", (void *)1);
	TEST("%p", (void *)-1); // 최대 주소값 (uintptr_t 최대)

	section("%u");
	TEST("%u", 0u);
	TEST("%u", 1u);
	TEST("%u", 42u);
	TEST("%u", 2147483647u);
	TEST("%u", 2147483648u); // INT_MAX + 1
	TEST("%u", 4294967295u); // UINT_MAX

	section("%x / %X");
	TEST("%x", 0);
	TEST("%x", 1);
	TEST("%x", 10);
	TEST("%x", 42);
	TEST("%x", 255);
	TEST("%x", 4294967295u);
	TEST("%X", 0);
	TEST("%X", 10);
	TEST("%X", 255);
	TEST("%X", 4294967295u);

	section("%d / %i");
	TEST("%d", 0);
	TEST("%d", 1);
	TEST("%d", -1);
	TEST("%d", 42);
	TEST("%d", -42);
	TEST("%d", 2147483647);       // INT_MAX
	TEST("%d", (int)-2147483648); // INT_MIN
	TEST("%i", 0);
	TEST("%i", 1);
	TEST("%i", -1);
	TEST("%i", 2147483647);
	TEST("%i", (int)-2147483648);

	section("%%");
	TEST("%%");
	TEST("100%%");
	TEST("%%d");

	section("no specifier");
	TEST("no format");
	TEST("");
	TEST("hello\nworld");

	section("mixed");
	TEST("%c %s %d %i %u %x %X %%", 'Z', "42", -1, -1, 100u, 255, 255);
	TEST("%d %d %d", 0, INT_MAX, (int)-2147483648);
	TEST("%s%s%s", "a", "b", "c");
	TEST("%p %p", &num, null_ptr);
}
