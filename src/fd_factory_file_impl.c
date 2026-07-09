/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_factory_file_impl.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:13:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 14:15:44 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "fd_factory.h"
#include <fcntl.h>

int	create_file_fd_in(t_fd_factory *this)
{
	int	fd;

	fd = open(this->input, O_RDONLY);
	if (fd < 0)
	{
		report_error(this->input, ERR_SYSCALL);
		fd = open("/dev/null", O_RDONLY);
	}
	return (fd);
}
