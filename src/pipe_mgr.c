/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_mgr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 21:01:14 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 12:13:14 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_mgr.h"
#include <unistd.h>


void	close_other_pipes_impl(t_pipe_mgr *this)
{
}

void	destroy_impl(t_pipe_mgr *this)
{
	free(this->pipes);
}

t_error	pipe_mgr_init(t_pipe_mgr *this, int _pipe_num)
{
	int	i;

	this->close_other_pipes = close_other_pipes_impl;
	this->destroy = destroy_impl;
	this->pipe_num = _pipe_num;
	this->pipes = malloc(this->pipe_num * sizeof(int [2]));
	if (!this->pipes)
		return (set_error(ERR_MALLOC_FAILED));
	i = -1;
	while (++i)
		pipe(this->pipes[i]);
	return (set_error(ERR_OK));
}
