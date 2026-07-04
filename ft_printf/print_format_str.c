/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:18:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 15:21:31 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>
#include <stddef.h>

int	print_format_c(va_list args)
{
	char	c;
	int		len;

	c = va_arg(args, int);
	len = write(1, &c, 1);
	if (len < 0)
		return (-1);
	return (len);
}

int	print_format_s(va_list args)
{
	char	*s;
	int		len;

	s = va_arg(args, char *);
	if (s == NULL)
		s = "(null)";
	len = write(1, s, ft_strlen(s));
	if (len < 0)
		return (-1);
	return (len);
}

int	print_format_percent(va_list args)
{
	int	len;

	(void)args;
	len = write(1, "%", 1);
	if (len < 0)
		return (-1);
	return (1);
}
