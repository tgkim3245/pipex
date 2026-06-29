/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/29 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "error.h"
#include "parser.h"
#include <stdlib.h>

static t_error	app_run_impl(t_app *this)
{
	(void)this;
	// cmd 갯수만큼 파이프 만들고, fork
	return (set_error(ERR_OK));
}

static void	app_destroy_impl(t_app *this)
{
	if (this->parser)
		this->parser->destroy(this->parser);
}

t_error	app_init(t_app *this, int argc, char **argv)
{
	t_parse_result	*result;

	this->run = app_run_impl;
	this->destroy = app_destroy_impl;
	this->parser = parser_create(argc, argv);
	if (!this->parser)
		return (get_error());
	if (this->parser->parse(this->parser) != ERR_OK)
		return (get_error());
	result = &this->parser->result;
	if (pipe_mgr_init(&this->pipe_mgr, result->pipe_num) != ERR_OK)
		return (get_error());
	if (command_mgr_init(&this->cmd_mgr, result->command_num,
			result->commands) != ERR_OK)
		return (get_error());
	return (set_error(ERR_OK));
}
