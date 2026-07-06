/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 18:09:46 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "parser.h"
#include <stdlib.h>

static t_status	app_run_impl(t_app *this)
{
	if (this->reader.read(&this->reader) != OK)
		return (FAIL);
	if (this->cmd_mgr.run(&this->cmd_mgr) != OK)
		return (FAIL);
	if (this->writer.write(&this->writer) != OK)
		return (FAIL);
	return (OK);
}

static void	app_destroy_impl(t_app *this)
{
	if (this->cmd_mgr.destroy)
		this->cmd_mgr.destroy(&this->cmd_mgr);
	if (this->writer.destroy)
		this->writer.destroy(&this->writer);
	if (this->reader.destroy)
		this->reader.destroy(&this->reader);
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
	if (pipe_mgr_init(&this->pipe_mgr, this->parser.parsed.command_num + 1)
		!= OK)
		return (FAIL);
	if (reader_init(&this->reader, this->pipe_mgr.pipes[0]) != OK)
		return (FAIL);
	if (writer_init(&this->writer,
			this->pipe_mgr.pipes[this->parser.parsed.command_num]) != OK)
		return (FAIL);
	if (cmd_mgr_init(&this->cmd_mgr, &this->parser.parsed, &this->pipe_mgr,
			envp) != OK)
		return (FAIL);
	return (OK);
}
