/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:40:07 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 13:56:09 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "error.h"
#include <stdlib.h>

t_error	app_run_impl(t_app *this)
{
	(void) this;
	return (ERR_OK);
}

void	app_destroy_impl(t_app *this)
{
	(void) this;
}

t_error	app_init(t_app *this, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	this->run = app_run_impl;
	this->destroy = app_destroy_impl;
	// cmd 몇개가 들어올지 모름
	// 파싱해서 cmd init
	this->cmd = malloc(this->cmd_num * sizeof(t_cmd));
	if (!this->cmd)
		return (set_error(ERR_MALLOC_FAILED));


	return (ERR_OK);
}
