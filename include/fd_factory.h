/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_factory.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 12:40:45 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 14:36:32 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_FACTORY_H
# define FD_FACTORY_H

# include "parser.h"
# include "types.h"

struct				s_fd_factory
{
	t_input_type	input_type;
	char			*input;
	char			*outfile_name;

	int				(*create_fd_in)(t_fd_factory *this);
	int				(*create_fd_out)(t_fd_factory *this);
	void			(*destroy)(t_fd_factory *this);
};

t_status			fd_factory_init(t_fd_factory *this, t_parsed *parsed);

int					create_file_fd_in(t_fd_factory *this);
int					create_heredoc_fd_in(t_fd_factory *this);

#endif // FD_FACTORY_H