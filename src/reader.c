/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:33:24 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 14:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include "error.h"
#include <unistd.h>

static void	destroy_impl(t_reader *this)
{
	close(this->fd_in);
}

t_status	reader_init(t_reader *this, t_parsed *parsed)
{
	static int	(*const create_fd_in[TYPE_COUNT])(t_parsed *parsed) = {
	[TYPE_FILE] = create_file_fd_in,
	[TYPE_HEREDOC] = create_heredoc_fd_in,
	};

	if (parsed->input_type >= TYPE_COUNT)
		return (report_error("reader_init", ERR_INVALID_READ_TYPE));
	this->destroy = destroy_impl;
	this->fd_in = create_fd_in[parsed->input_type](parsed);
	return (OK);
}
