/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:54:22 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 17:17:27 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

static void	_init_print_format(t_print_format *pf)
{
	pf['c'] = print_format_c;
	pf['s'] = print_format_s;
	pf['p'] = print_format_p;
	pf['d'] = print_format_d;
	pf['i'] = print_format_i;
	pf['u'] = print_format_u;
	pf['x'] = print_format_x;
	pf['X'] = print_format_upper_x;
	pf['%'] = print_format_percent;
}

int	ft_printf(const char *str, ...)
{
	int				len;
	int				ret;
	va_list			args;
	t_print_format	print_format[256];

	_init_print_format(print_format);
	va_start(args, str);
	len = 0;
	while (*str != '\0')
	{
		if (*str != '%')
			ret = write(1, str, 1);
		else
			ret = print_format[(unsigned char)*(++str)](args);
		if (ret < 0)
		{
			len = -1;
			break ;
		}
		len += ret;
		str++;
	}
	va_end(args);
	return (len);
}
