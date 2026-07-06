/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 18:26:42 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_H
# define READER_H

# include "parser.h"
# include <sys/types.h>

struct s_reader
{
	pid_t	pid;

	int		fd_in;
	int		fd_out;

	char	(*read)(t_reader *this);
	void	(*destroy)(t_reader *this);
};

t_status	reader_init(t_reader *this, int fd[2]);

#endif
