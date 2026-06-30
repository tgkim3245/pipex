/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/30 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITER_H
# define WRITER_H

# include "error.h"

struct s_writer
{
	char	(*write)(t_writer *this);
	void	(*destroy)(t_writer *this);
};

t_error	writer_init(t_writer *this, char *outfile_name, int append);

#endif
