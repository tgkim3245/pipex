/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:18:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/23 17:21:26 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	calc_min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	size_t	m_size;
	char	*ret;

	s_len = ft_strlen(s);
	if (start < s_len)
		m_size = calc_min(s_len - start, len);
	else
		m_size = 0;
	ret = (char *)malloc(m_size + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (i < m_size)
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
