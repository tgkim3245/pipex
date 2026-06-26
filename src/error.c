/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 15:20:49 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <stdio.h>

static t_error	*get_lasterr(void)
{
	static __thread t_error	lasterr = {ERR_OK};

	return (&lasterr);
}

t_error	get_error(void)
{
	t_error	*lasterr;

	lasterr = get_lasterr();
	return (*lasterr);
}

t_error	set_error(t_error err)
{
	t_error	*lasterr;

	lasterr = get_lasterr();
	*lasterr = err;
	return (err);
}

void	print_error(void)
{
	static const char	*error_msg[] = {
	[ERR_OK] = " Success",
	};
	t_error				*lasterr;

	lasterr = get_lasterr();
	printf("%s\n", error_msg[*lasterr]);
	set_error(ERR_OK);
}
