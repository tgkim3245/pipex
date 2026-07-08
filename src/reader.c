/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:33:24 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 22:59:31 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include "error.h"
#include <sys/wait.h>
#include <unistd.h>

static void	destroy_impl(t_reader *this)
{
	if (this->fd_in != STDIN_FILENO)
		close(this->fd_in);
	waitpid(this->pid, NULL, 0);
}

t_status	reader_init(t_reader *this, t_parsed *parsed, t_pipe_mgr *pm)
{
	static const t_reader_ops	ops[TYPE_COUNT] = {
	[TYPE_FILE] = {create_file_fd_in, read_file_impl},
	[TYPE_HEREDOC] = {create_heredoc_fd_in, read_heredoc_impl},
	};

	if (parsed->input_type >= TYPE_COUNT)
		return (report_error("reader_init", ERR_INVALID_READ_TYPE));
	this->destroy = destroy_impl;
	this->parsed = parsed;
	this->pm = pm;
	this->fd_out = pm->pipes[0][1];
	this->fd_in = ops[parsed->input_type].create_fd_in(parsed);
	this->read = ops[parsed->input_type].read;
	return (OK);
}
