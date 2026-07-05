/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 13:31:16 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:23:40 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include <unistd.h>
#include "error.h"

static t_error	run_impl(t_cmd *this)
{
	execve(this->path, this->argv, this->envp);
	print_error("execve failed", ERR_SYSCALL);
	exit(1);
}

static void	destroy_impl(t_cmd *this)
{
}

t_error	cmd_init(t_cmd this, char *cmd_str, char **envp, int fd_in, int fd_out)
{
	this->run = run_impl;
	this->destroy = destroy_impl;
}
