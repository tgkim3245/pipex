/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/25 15:40:15 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "error.h"
#include "parser.h"
#include <stdlib.h>

t_error	app_run_impl(t_app *this)
{
	(void)this;
	// cmd 갯수만큼 파이프 만들고, fork
	return (set_error(ERR_OK));
}

void	app_destroy_impl(t_app *this)
{
	(void)this;
}

t_error	app_init(t_app *this, int argc, char **argv)
{
	t_parse_result	parsed;

	this->run = app_run_impl;
	this->destroy = app_destroy_impl;
	if (parser_init(&this->parser, argc, argv) != ERR_OK)
		return (get_error());
	if (this->parser.parse(&this->parser, &parsed) != ERR_OK)
		return (get_error());

	if (pipe_mgr_init(&this->pipe_mgr, parsed.pipe_num) != ERR_OK)
		return (get_error());
	if (command_mgr_init(&this->cmd_mgr, parsed.command_num) != ERR_OK)
		return (get_error());
	return (set_error(ERR_OK));
}
