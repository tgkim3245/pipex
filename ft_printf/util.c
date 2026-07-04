/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 16:13:28 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 16:08:16 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdint.h>
#include <stdlib.h>

static long long	_ft_abs(long long n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

/*
- count digit lenght (except '-')
- alway return 1 >=
	ex : 0 -> return 1
*/
unsigned int	cnt_base_x(long long n, unsigned int base)
{
	int	ret;

	if (n == 0)
		return (1);
	ret = 0;
	while (n != 0)
	{
		ret++;
		n /= base;
	}
	return (ret);
}

int	nbr2base_str(long long nbr, char **str, unsigned int base)
{
	unsigned int	len;
	int				idx;

	len = cnt_base_x(nbr, base);
	if (nbr < 0)
		len++;
	(*str) = (char *)malloc(len + 1);
	if (!(*str))
		return (-1);
	(*str)[len] = '\0';
	if (nbr < 0)
		(*str)[0] = '-';
	else if (nbr == 0)
		(*str)[0] = '0';
	nbr = _ft_abs(nbr);
	idx = len - 1;
	while (nbr != 0)
	{
		(*str)[idx--] = "0123456789abcdef"[nbr % base];
		nbr /= base;
	}
	return (len);
}

unsigned int	cnt_ptr_base_x(uintptr_t n, unsigned int base)
{
	int	ret;

	if (n == 0)
		return (1);
	ret = 0;
	while (n != 0)
	{
		ret++;
		n /= base;
	}
	return (ret);
}

int	ptr2base_str(uintptr_t nbr, char **str, unsigned int base)
{
	int	len;
	int	idx;

	len = cnt_ptr_base_x(nbr, base);
	(*str) = (char *)malloc(len + 1);
	if (!(*str))
		return (-1);
	(*str)[len] = '\0';
	if (nbr == 0)
		(*str)[0] = '0';
	idx = len - 1;
	while (nbr != 0)
	{
		(*str)[idx--] = "0123456789abcdef"[nbr % base];
		nbr /= base;
	}
	return (len);
}
