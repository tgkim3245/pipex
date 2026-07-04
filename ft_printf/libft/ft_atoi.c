/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:45:40 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/23 17:18:50 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(char c)
{
	char	*spaces;
	int		i;

	spaces = "\t\n\v\f\r ";
	i = 0;
	while (i < 6)
	{
		if (c == spaces[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		ret;
	int		buho;

	i = 0;
	buho = 1;
	ret = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			buho = -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		ret = ret * 10 + nptr[i] - '0';
		i++;
	}
	return (ret * buho);
}
