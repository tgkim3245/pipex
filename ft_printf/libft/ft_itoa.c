/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 22:57:39 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/23 17:19:36 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_abs(long long n)
{
	if (n >= 0)
		return ((unsigned int)n);
	else
		return ((unsigned int)(-n));
}

static void	slice_int2str(unsigned int n, size_t len, char *s)
{
	while (1)
	{
		s[--len] = (n % 10) + '0';
		n /= 10;
		if (n == 0)
			break ;
	}
}

static size_t	cnt_digit(int n)
{
	size_t	cnt;

	if (n == 0)
		return (1);
	cnt = 0;
	while (n != 0)
	{
		cnt++;
		n /= 10;
	}
	return (cnt);
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*p;

	len = cnt_digit((long long)n);
	if (n < 0)
		len += 1;
	p = (char *)malloc(len + 1);
	if (!p)
		return (NULL);
	slice_int2str(ft_abs(n), len, p);
	if (n < 0)
		p[0] = '-';
	p[len] = '\0';
	return (p);
}
