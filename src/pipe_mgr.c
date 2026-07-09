/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_mgr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:01:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 16:57:09 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_mgr.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static void	close_all_pipes_impl(t_pipe_mgr *this)
{
	int	i;

	if (!this->pipes)
		return ;
	i = -1;
	while (++i < this->pipe_num)
	{
		if (this->pipes[i][0] >= 0)
			close(this->pipes[i][0]);
		if (this->pipes[i][1] >= 0)
			close(this->pipes[i][1]);
	}
}

static void	destroy_impl(t_pipe_mgr *this)
{
	this->close_all_pipes(this);
	free(this->pipes);
	this->pipes = NULL;
	if (this->fd_f.destroy)
		this->fd_f.destroy(&this->fd_f);
}

t_status	pipe_mgr_init(t_pipe_mgr *this, t_parsed *parsed)
{
	int	i;

	this->close_all_pipes = close_all_pipes_impl;
	this->destroy = destroy_impl;
	this->pipe_num = parsed->command_num + 1;
	this->pipes = ft_calloc(this->pipe_num, sizeof(int [2]));
	if (!this->pipes)
		return (report_error("pipe_mgr_init", ERR_SYSCALL));
	ft_memset(this->pipes, -1, this->pipe_num * sizeof(int [2]));
	if (fd_factory_init(&this->fd_f, parsed) != OK)
		return (FAIL);
	this->pipes[0][0] = this->fd_f.create_fd_in(&this->fd_f);
	i = 0;
	while (++i < this->pipe_num - 1)
		if (pipe(this->pipes[i]) == -1)
			return (report_error("pipe_create", ERR_SYSCALL));
	this->pipes[this->pipe_num - 1][1] = this->fd_f.create_fd_out(&this->fd_f);
	return (OK);
}
