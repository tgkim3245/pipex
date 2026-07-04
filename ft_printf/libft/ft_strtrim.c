/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 17:13:10 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/23 17:21:16 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	find_start(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0')
	{
		if (!ft_strchr(set, s1[i]))
			break ;
		i++;
	}
	return (i);
}

static size_t	find_end(char const *s1, char const *set)
{
	size_t	i;

	i = ft_strlen(s1);
	while (i > 0)
	{
		if (!ft_strchr(set, s1[i - 1]))
			break ;
		i--;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	slen;
	char	*p;

	start = find_start(s1, set);
	end = find_end(s1, set);
	if (start < end)
		slen = end - start;
	else
		slen = 0;
	p = (char *)malloc(slen + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s1 + start, slen);
	p[slen] = '\0';
	return (p);
}
