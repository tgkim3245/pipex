/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:01 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 16:55:30 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "types.h"

typedef enum e_error
{
	ERR_SYSCALL,
	ERR_CREATE_PARSER_FAILED,
	ERR_INVALID_ARGS,
	ERR_CREATE_COMMAND_FAILED,
	ERR_CREATE_PATH_FAILED,
	ERR_ARGV_SPLIT_FAILED,
	ERR_INVALID_READ_TYPE,
}		t_error;

t_status	report_error(char *err_title, t_error err_type);

#endif
