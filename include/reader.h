/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 14:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_H
# define READER_H

# include "parser.h"
# include "types.h"

struct		s_reader
{
	int		fd_in;

	void	(*destroy)(t_reader *this);
};

t_status	reader_init(t_reader *this, t_parsed *parsed);

int			create_file_fd_in(t_parsed *parsed);
int			create_heredoc_fd_in(t_parsed *parsed);

#endif
