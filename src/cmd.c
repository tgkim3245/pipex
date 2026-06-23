/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:48:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 12:44:34 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"

t_error	cmd_run_impl(t_cmd *this)
{
	(void) this;
	return (ERR_OK);
}

void	cmd_destroy_impl(t_cmd *this)
{
	(void) this;
}

t_error	cmd_init(t_cmd *this)
{
	this->run = cmd_run_impl;
	this->destroy = cmd_destroy_impl;
	return (ERR_OK);
}
