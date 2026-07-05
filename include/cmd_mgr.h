/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mgr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:47:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:49:12 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_MGR_H
# define CMD_MGR_H

# include "error.h"
# include "pipe_mgr.h"
# include "cmd.h"

struct			s_cmd_mgr
{
	int			command_num;
	t_cmd		*commands;

	t_pipe_mgr	*pm;

	t_cmd 		*cmd_create(char *cmd_str, char **envp, int fd_in, int fd_out);
	void		(*destroy)(t_cmd_mgr *this);
};

t_status			cmd_mgr_init(t_cmd_mgr *this, t_parse_result parsed,
					t_pipe_mgr _pm, char **envp);

#endif // CMD_MGR_H
