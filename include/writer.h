/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 18:26:57 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITER_H
# define WRITER_H

# include "error.h"

struct s_writer
{
	pid_t	pid;

	int		fd_in;
	int		fd_out;

	char	(*write)(t_writer *this);
	void	(*destroy)(t_writer *this);
};

t_status	writer_init(t_writer *this, int fd[2]);

#endif
