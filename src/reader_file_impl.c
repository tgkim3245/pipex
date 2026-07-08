/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader_file_impl.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:13:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 23:15:24 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "reader.h"
#include <fcntl.h>
#include <unistd.h>

#define READ_BUF_SIZE 4096

static t_status	copy_loop(int fd_in, int fd_out)
{
	char	buf[READ_BUF_SIZE];
	ssize_t	n;

	n = read(fd_in, buf, READ_BUF_SIZE);
	while (n > 0)
	{
		if (write(fd_out, buf, n) < 0)
			return (report_error("write", ERR_SYSCALL));
		n = read(fd_in, buf, READ_BUF_SIZE);
	}
	if (n < 0)
		return (report_error("read", ERR_SYSCALL));
	return (OK);
}

t_status	read_file_impl(t_reader *this)
{
	this->pid = fork();
	if (this->pid < 0)
		return (report_error("fork", ERR_SYSCALL));
	if (this->pid == 0)
	{
		dup2(this->fd_in, STDIN_FILENO);
		close(this->fd_in);
		dup2(this->fd_out, STDOUT_FILENO);
		close(this->fd_out);
		this->pm->close_all_pipes(this->pm);
		if (copy_loop(STDIN_FILENO, STDOUT_FILENO) != OK)
			_exit(1);
		_exit(0);
	}
	return (OK);
}

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
