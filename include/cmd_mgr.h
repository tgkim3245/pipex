/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mgr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:47:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 16:04:34 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_MGR_H
# define CMD_MGR_H

# include "cmd.h"
# include "error.h"
# include "pipe_mgr.h"

struct			s_cmd_mgr
{
	t_cmd		*cmds;

	int			cmd_num;
	int			exit_code;
	t_pipe_mgr	*pm;

	t_status	(*run)(t_cmd_mgr * this);
	void		(*destroy)(t_cmd_mgr *this);
};

t_status	cmd_mgr_init(t_cmd_mgr *this, t_parsed *parsed, t_pipe_mgr *_pm,
				char **envp);

#endif // CMD_MGR_H
