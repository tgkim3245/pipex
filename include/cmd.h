/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:47:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 12:40:11 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "types.h"
# include "error.h"

typedef struct s_cmd
{
	t_error				(*run)(t_cmd * this);
	void				(*destroy)(t_cmd *this);
}						t_cmd;

t_error					cmd_init(t_cmd *this);
#endif