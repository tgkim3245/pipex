/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mgr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:48:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 18:07:44 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_mgr.h"
#include "libft.h"
#include "error.h"
#include <stddef.h>
#include <sys/wait.h> 

static t_status	run_impl(t_cmd_mgr *this)
{
	int	i;

	i = -1;
	while (++i < this->cmd_num)
		if (this->cmds[i].run(&this->cmds[i]) != OK)
			return (FAIL);
	this->pm->close_all_pipes(this->pm);
	return (OK);
}

static void	destroy_impl(t_cmd_mgr *this)
{
	int	i;

	i = -1;
	while (++i)
		waitpid(this->cmds[i].pid, NULL, 0);
	i = -1;
	while (++i < this->cmd_num)
		if (!&this->cmds[i])
			this->cmds[i].destroy(&this->cmds[i]);
	free(this->cmds);
	this->cmds = NULL;
}

t_status	cmd_mgr_init(t_cmd_mgr *this, t_parsed *parsed,
				t_pipe_mgr *_pm, char **_envp)
{
	int	i;

	this->run = run_impl;
	this->destroy = destroy_impl;
	this->cmd_num = parsed->command_num;
	this->pm = _pm;
	this->cmds = ft_calloc(this->cmd_num, sizeof(t_cmd));
	if (!this->cmds)
		report_error("cmd_mgr_init", ERR_SYSCALL);
	i = -1;
	while (++i < this->cmd_num)
		if (cmd_init(&this->cmds[i], i, parsed, _pm, _envp) != OK)
			return (FAIL);
	return (OK);
}
