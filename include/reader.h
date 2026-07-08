/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 23:40:08 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_H
# define READER_H

# include "parser.h"
# include "pipe_mgr.h"
# include "types.h"
# include <sys/types.h>

struct			s_reader
{
	pid_t		pid;
	int			fd_in;

	t_parsed	*parsed;
	t_pipe_mgr	*pm;
	int			fd_out;

	t_status	(*read)(t_reader *this);
	void		(*destroy)(t_reader *this);
};

typedef struct s_reader_ops
{
	int			(*create_fd_in)(t_parsed *parsed);
	t_status	(*read)(t_reader *this);
}				t_reader_ops;

t_status		reader_init(t_reader *this, t_parsed *parsed, t_pipe_mgr *_pm);

t_status		read_file_impl(t_reader *this);
int				create_file_fd_in(t_parsed *parsed);
t_status		read_heredoc_impl(t_reader *this);
int				create_heredoc_fd_in(t_parsed *parsed);

#endif
