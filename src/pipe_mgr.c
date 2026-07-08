/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_mgr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:01:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 13:49:14 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_mgr.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static void	close_all_pipes_impl(t_pipe_mgr *this)
{
	int	i;

	i = -1;
	while (++i < this->pipe_num)
	{
		if (this->pipes[i] == NULL)
			continue ;
		close(this->pipes[i][0]);
		close(this->pipes[i][1]);
	}
}

static void	destroy_impl(t_pipe_mgr *this)
{
	this->close_all_pipes(this);
	free(this->pipes);
}


t_status	pipe_mgr_init(t_pipe_mgr *this, int _pipe_num)
{
	int	i;

	this->close_all_pipes = close_all_pipes_impl;
	this->destroy = destroy_impl;
	this->pipe_num = _pipe_num;
	this->pipes = ft_calloc(this->pipe_num, sizeof(int [2]));
	if (!this->pipes)
		return (report_error("pipe_mgr_init", ERR_SYSCALL));
	i = -1;
	while (++i < this->pipe_num)
		if (pipe(this->pipes[i]) == -1)
			return (report_error("pipe", ERR_SYSCALL));
	return (OK);
}
