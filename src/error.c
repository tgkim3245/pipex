/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 16:52:43 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

t_status	report_error(char *err_title, t_error err_type)
{
	static const char	*error_msg[] = {
	[ERR_SYSCALL] = "you can't display this message, something wrong.",
	[ERR_CREATE_PARSER_FAILED] = "failed to create parser",
	[ERR_INVALID_ARGS] = "invalid arguments",
	[ERR_CREATE_COMMAND_FAILED] = "failed to create command",
	[ERR_CREATE_PATH_FAILED] = "command not found",
	[ERR_ARGV_SPLIT_FAILED] = "failed to split command arguments",
	};
	const char			*msg;

	if (err_title == NULL)
		err_title = "Error";
	if (err_type == ERR_SYSCALL)
		msg = strerror(errno);
	else
		msg = error_msg[err_type];
	ft_putstr_fd(err_title, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd((char *)msg, STDERR_FILENO);
	return (FAIL);
}
