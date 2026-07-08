/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:34:57 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 14:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "writer.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>

static void	destroy_impl(t_writer *this)
{
	close(this->fd_out);
}

t_status	writer_init(t_writer *this, t_parsed *parsed)
{
	this->destroy = destroy_impl;
	this->fd_out = open(parsed->outfile_name, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (this->fd_out < 0)
	{
		report_error(parsed->outfile_name, ERR_SYSCALL);
		this->fd_out = open("/dev/null", O_WRONLY);
	}
	return (OK);
}
