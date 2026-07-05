/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 13:31:46 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:49:37 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "reader.h"
# include "writer.h"
# include "error.h"

struct			s_cmd
{
	t_reader	reader;
	t_writer	writer;

	char		*path;
	char		**argv;
	char		**envp;

	t_status	(*run)(t_cmd * this);
	void		(*destroy)(t_cmd *this);
};

t_status	cmd_init(t_cmd this, char *cmd_str, char **envp, int fd_in, int fd_out);


#endif // CMD_H