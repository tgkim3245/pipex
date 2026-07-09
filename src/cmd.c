/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 13:31:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 16:18:59 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "cmd.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include "error.h"
#include "util.h"

static t_status	run_impl(t_cmd *this)
{
	this->pid = fork();
	if (this->pid < 0)
		return (report_error("fork failed", ERR_SYSCALL));
	else if (this->pid == 0)
	{
		dup2(this->fd_in, STDIN_FILENO);
		close(this->fd_in);
		dup2(this->fd_out, STDOUT_FILENO);
		close(this->fd_out);
		this->pm->close_all_pipes(this->pm);
		execve(this->path, this->argv, this->envp);
		if (!this->found && !ft_strchr(this->argv[0], '/'))
			report_error(this->argv[0], ERR_CREATE_PATH_FAILED);
		else
			report_error(this->argv[0], ERR_SYSCALL);
		_exit(127);
	}
	else
		return (OK);
}

static void	destroy_impl(t_cmd *this)
{
	free(this->path);
	free_split(this->argv);
}

t_status	cmd_init(t_cmd *this, int idx, const t_parsed *parsed,
					t_pipe_mgr *_pm, char **_envp)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
	this->pm = _pm;
	this->argv = ft_split(parsed->commands[idx], ' ');
	if (!this->argv)
		return (report_error("argv_split_failed", ERR_ARGV_SPLIT_FAILED));
	this->path = create_cmd_path(this->argv[0], _envp);
	this->found = (this->path != NULL);
	if (!this->found)
	{
		this->path = ft_strdup(this->argv[0]);
		if (!this->path)
			return (free_split(this->argv),
				report_error("cmd_init", ERR_SYSCALL));
	}
	this->fd_in = _pm->pipes[idx][0];
	this->fd_out = _pm->pipes[idx + 1][1];
	this->envp = _envp;
	return (OK);
}
