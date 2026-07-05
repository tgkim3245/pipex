/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 13:23:27 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include <stdlib.h>

static t_error	app_run_impl(t_app *this)
{
	(void)this;
	return (set_error(ERR_OK));
}

static void	app_destroy_impl(t_app *this)
{
	this->cmd_mgr.destroy(&this->cmd_mgr);
	this->pipe_mgr.destroy(&this->pipe_mgr);
}

t_error	app_init(t_app *this, int argc, char **argv, char **envp)
{
	t_parse_result	parsed;
	t_parser		*parser;

	this->run = app_run_impl;
	this->destroy = app_destroy_impl;
	parser = parser_create(argc, argv);
	if (!parser)
		return (get_error());
	if (parser->parse(parser, &parsed) != ERR_OK)
		return (parser->destroy(parser), get_error());
	parser->destroy(parser);
	if (pipe_mgr_init(&this->pipe_mgr, parsed.pipe_num) != ERR_OK)
		return (get_error());
	if (cmd_mgr_init(&this->cmd_mgr, parsed, this->pipe_mgr, envp) != ERR_OK)
		return (get_error());
	return (set_error(ERR_OK));
}
