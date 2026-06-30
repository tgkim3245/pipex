/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:12:03 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/30 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "command.h"
# include "pipe_mgr.h"
# include "reader.h"
# include "writer.h"

struct s_app
{
	t_command_mgr	cmd_mgr;
	t_pipe_mgr		pipe_mgr;
	t_reader		reader;
	t_writer		writer;
	t_error			(*run)(t_app *this);
	void			(*destroy)(t_app *this);
};

t_error	app_init(t_app *this, int argc, char **argv, char **envp);

#endif
