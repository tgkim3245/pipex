/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:38:01 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 12:41:14 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "types.h"

enum e_error
{
	ERR_OK,
	ERR_MALLOC_FAILED,
};

t_error	get_error(void);
t_error	set_error(t_error err);
void	print_error(void);

#endif