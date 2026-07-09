/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 14:47:45 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "parser.h"
#include <stdlib.h>

static t_status	app_run_impl(t_app *this)
{
	if (this->cmd_mgr.run(&this->cmd_mgr) != OK)
		return (FAIL);
	return (OK);
}

static void	app_destroy_impl(t_app *this)
{
	if (this->cmd_mgr.destroy)
		this->cmd_mgr.destroy(&this->cmd_mgr);
	if (this->pipe_mgr.destroy)
		this->pipe_mgr.destroy(&this->pipe_mgr);
	if (this->parser.destroy)
		this->parser.destroy(&this->parser);
}

t_status	app_init(t_app *this, int argc, char **argv, char **envp)
{
	this->run = app_run_impl;
	this->destroy = app_destroy_impl;
	if (parser_init(&this->parser, argc, argv) != OK)
		return (FAIL);
	if (this->parser.parse(&this->parser) != OK)
		return (this->parser.destroy(&this->parser), FAIL);
	if (pipe_mgr_init(&this->pipe_mgr, &this->parser.parsed) != OK)
		return (FAIL);
	if (cmd_mgr_init(&this->cmd_mgr, &this->parser.parsed, &this->pipe_mgr,
			envp) != OK)
		return (FAIL);
	return (OK);
}
