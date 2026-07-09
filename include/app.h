/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:12:03 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 14:16:04 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "cmd_mgr.h"
# include "pipe_mgr.h"
# include "parser.h"

struct s_app
{
	t_parser		parser;
	t_pipe_mgr		pipe_mgr;
	t_cmd_mgr		cmd_mgr;

	t_status		(*run)(t_app * this);
	void			(*destroy)(t_app *this);
};

t_status	app_init(t_app *this, int argc, char **argv, char **envp);

#endif
