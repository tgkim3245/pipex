/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_mgr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:48:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:50:57 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_mgr.h"

t_status	cmd_run_impl(t_cmd *this)
{
	(void)this;
	return (OK);
}

void	cmd_destroy_impl(t_cmd *this)
{
	(void)this;
}

t_cmd	*cmd_create(char *cmd_str, char **envp, int fd_in, int fd_out)
{
}

t_status	cmd_mgr_init(t_cmd_mgr *this, t_parse_result parsed, t_pipe_mgr _pm,
		char **envp)
{
	this->destroy = cmd_destroy_impl;
	return (ERR_OK);
}
