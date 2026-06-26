/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:48:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/25 14:55:18 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

t_error	command_run_impl(t_command *this)
{
	(void) this;
	return (ERR_OK);
}

void	command_destroy_impl(t_command *this)
{
	(void) this;
}

t_error	command_init(t_command *this)
{
	this->run = command_run_impl;
	this->destroy = command_destroy_impl;
	return (ERR_OK);
}
