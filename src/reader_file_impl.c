/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader_file_impl.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:13:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/08 14:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "reader.h"
#include <fcntl.h>

int	create_file_fd_in(t_parsed *parsed)
{
	int	fd;

	fd = open(parsed->input, O_RDONLY);
	if (fd < 0)
	{
		report_error(parsed->input, ERR_SYSCALL);
		fd = open("/dev/null", O_RDONLY);
	}
	return (fd);
}
