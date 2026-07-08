/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 14:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITER_H
# define WRITER_H

# include "parser.h"
# include "types.h"

struct		s_writer
{
	int		fd_out;

	void	(*destroy)(t_writer *this);
};

t_status	writer_init(t_writer *this, t_parsed *parsed);

#endif
