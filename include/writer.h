/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 23:15:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITER_H
# define WRITER_H

# include "parser.h"
# include "pipe_mgr.h"
# include "types.h"
# include <sys/types.h>

struct			s_writer
{
	pid_t		pid;

	t_pipe_mgr	*pm;
	int			fd_in;
	int			fd_out;

	t_status	(*write)(t_writer *this);
	void		(*destroy)(t_writer *this);
};

t_status		writer_init(t_writer *this, t_parsed *parsed, t_pipe_mgr *pm);

#endif
