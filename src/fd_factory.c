/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_factory.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 12:40:48 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/10 12:20:58 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fd_factory.h"
#include <fcntl.h>

static void	destroy_impl(t_fd_factory *this)
{
	(void)this;
}

static int	create_fd_in_impl(t_fd_factory *this)
{
	if (this->input_type == TYPE_FILE)
		return (create_file_fd_in(this));
	return (create_heredoc_fd_in(this));
}

static int	create_fd_out_impl(t_fd_factory *this)
{
	int	fd_out;
	int	flags;

	flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (this->input_type == TYPE_HEREDOC)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd_out = open(this->outfile_name, flags, 0644);
	if (fd_out < 0)
	{
		report_error(this->outfile_name, ERR_SYSCALL);
		fd_out = open("/dev/null", O_WRONLY);
	}
	return (fd_out);
}

t_status	fd_factory_init(t_fd_factory *this, t_parsed *parsed)
{
	this->destroy = destroy_impl;
	this->create_fd_in = create_fd_in_impl;
	this->create_fd_out = create_fd_out_impl;
	this->input_type = parsed->input_type;
	this->input = parsed->input;
	this->outfile_name = parsed->outfile_name;
	return (OK);
}
