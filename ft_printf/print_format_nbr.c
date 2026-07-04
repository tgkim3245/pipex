/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format_nbr.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 19:01:52 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 15:10:14 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int	print_format_u(va_list args)
{
	unsigned int	n;
	int				len;
	int				ret;
	char			*str;

	n = va_arg(args, unsigned int);
	ret = nbr2base_str(n, &str, 10);
	if (ret < 0)
		return (-1);
	len = write(1, str, ret);
	if (len < 0)
		return (-1);
	free(str);
	return (len);
}

int	print_format_d(va_list args)
{
	int		n;
	char	*str;
	int		ret;
	int		len;

	n = va_arg(args, int);
	ret = nbr2base_str((long long)n, &str, 10);
	if (ret < 0)
		return (-1);
	len = write(1, str, ft_strlen(str));
	if (len < 0)
		return (-1);
	free(str);
	return (len);
}

int	print_format_i(va_list args)
{
	return (print_format_d(args));
}
