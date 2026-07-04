/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 15:44:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/23 17:20:06 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Unlike memcpy, this function is safe when memory regions overlap.
If dest and src overlap, copying can overwrite source bytes during the move.
ft_memmove handles this case by choosing a safe copy direction.
*/
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (s < d)
		while (n--)
			d[n] = s[n];
	else if (d < s)
		while (n--)
			*d++ = *s++;
	return (dest);
}
