/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 13:31:46 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 23:19:35 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "reader.h"
# include "writer.h"
# include "error.h"
# include "pipe_mgr.h"
# include <stdbool.h>
# include <sys/types.h>

struct			s_cmd
{
	pid_t		pid;
	char		*path;
	bool		found;
	char		**argv;

	int			fd_in;
	int			fd_out;
	t_pipe_mgr	*pm;
	char		**envp;

	t_status	(*run)(t_cmd * this);
	void		(*destroy)(t_cmd *this);
};

t_status	cmd_init(t_cmd *this, int idx, const t_parsed *parsed,
				t_pipe_mgr *_pm, char **_envp);

#endif // CMD_H