/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:48:36 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "ft_printf.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>


t_status	report_error(char *err_title, t_error err_type)
{
	static const char	*error_msg[] = {
	[ERR_SYSCALL] = "can't display this message, something wrong.",
	[ERR_CREATE_PARSER_FAILED] = "CREATE_PARSER_FAILED",
	[ERR_INVALID_ARGS] = "ERR_INVALID_ARGS",
	};
	const char			*msg;

	if (err_title == NULL)
		err_title = "Error";
	if (err_type == ERR_SYSCALL)
		msg = strerror(errno);
	else
		msg = error_msg[err_type];
	ft_printf("%s: %s\n", err_title, msg);
	return (FAIL);
}
