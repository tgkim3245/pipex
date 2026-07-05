/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:01 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:48:20 by taegokim         ###   ########.fr       */
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
}		t_error;

t_status	report_error(char *err_title, t_error err_type);

#endif
