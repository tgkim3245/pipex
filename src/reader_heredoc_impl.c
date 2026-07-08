/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader_heredoc_impl.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:13:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 22:57:49 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "reader.h"
#include <stdlib.h>
#include <unistd.h>

static int	is_limiter(char *line, char *limiter)
{
	return (ft_strlen(line) == ft_strlen(limiter) && ft_strncmp(line, limiter,
			ft_strlen(limiter)) == 0);
}

static void	strip_newline(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static void	write_heredoc(int fd_out, char *limiter)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		strip_newline(line);
		if (is_limiter(line, limiter))
		{
			free(line);
			break ;
		}
		write(fd_out, line, ft_strlen(line));
		write(fd_out, "\n", 1);
		free(line);
	}
}

t_status	read_heredoc_impl(t_reader *this)
{
	this->pid = fork();
	if (this->pid < 0)
		return (report_error("fork", ERR_SYSCALL));
	if (this->pid == 0)
	{
		dup2(this->fd_out, STDOUT_FILENO);
		close(this->fd_out);
		this->pm->close_all_pipes(this->pm);
		write_heredoc(STDOUT_FILENO, this->parsed->input);
		_exit(0);
	}
	return (OK);
}

int	create_heredoc_fd_in(t_parsed *parsed)
{
	(void)parsed;
	return (STDIN_FILENO);
}
