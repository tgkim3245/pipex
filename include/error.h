/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:01 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/29 10:33:58 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "types.h"

typedef enum e_error
{
	ERR_OK,
	ERR_MALLOC_FAILED,
	ERR_CREATE_PARSER_FAILED,
	ERR_INVALID_ARGS,
}		t_error;

// typedef struct s_error
// {
// 	enum e_error_type	err;
// }	t_error;

t_error	get_error(void);
t_error	set_error(t_error err);
void	print_error(void);

#endif