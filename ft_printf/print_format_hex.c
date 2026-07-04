/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format_hex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 18:20:55 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 16:09:24 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int	print_format_p(va_list args)
{
	char	*hex_address;
	int		len;
	void	*p;

	len = 0;
	p = va_arg(args, void *);
	if (p == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	if (ptr2base_str((uintptr_t)p, &hex_address, 16) < 0)
		return (-1);
	if (write(1, "0x", 2) < 0)
		return (-1);
	len = write(1, hex_address, ft_strlen(hex_address));
	if (len < 0)
		return (-1);
	free(hex_address);
	return (len + 2);
}

int	print_format_x(va_list args)
{
	char			*str;
	int				len;
	unsigned int	n;
	int				ret;

	n = va_arg(args, unsigned int);
	ret = nbr2base_str((long long)n, &str, 16);
	if (ret < 0)
		return (-1);
	ret = write(1, str, ft_strlen(str));
	if (ret < 0)
		return (-1);
	len = ret;
	free(str);
	return (len);
}

int	print_format_upper_x(va_list args)
{
	char			*str;
	int				len;
	unsigned int	i;
	unsigned int	n;
	int				ret;

	n = va_arg(args, unsigned int);
	ret = nbr2base_str((long long)n, &str, 16);
	if (ret < 0)
		return (-1);
	i = 0;
	while (str[i] != '\0')
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
	ret = write(1, str, ft_strlen(str));
	if (ret < 0)
		return (-1);
	len = ret;
	free(str);
	return (len);
}
