/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:33:24 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 18:28:17 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"
#include <unistd.h>

static char	read_impl(t_reader *this)
{
	dup2(this->fd_in, STDIN_FILENO);
	close(this->fd_in);
}

static void	destroy_impl(t_reader *this)
{
}

t_status	reader_init(t_reader *this, int fd[2])
{
	this->read = read_impl;
	this->destroy = destroy_impl;
	this->fd_in = fd[0];
	this->fd_out = fd[1];
	return (OK);
}
