/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_factory_heredoc_impl.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:13:05 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 17:07:07 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "fd_factory.h"
#include <fcntl.h>
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

static char	*heredoc_tmp_path(void)
{
	char	*pid_str;
	char	*path;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	path = ft_strjoin("/tmp/.pipex_heredoc_", pid_str);
	free(pid_str);
	return (path);
}

int	create_heredoc_fd_in(t_fd_factory *this)
{
	char	*path;
	int		fd;

	path = heredoc_tmp_path();
	if (!path)
		return (open("/dev/null", O_RDONLY));
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		report_error(path, ERR_SYSCALL);
		free(path);
		return (open("/dev/null", O_RDONLY));
	}
	write_heredoc(fd, this->input);
	close(fd);
	fd = open(path, O_RDONLY);
	unlink(path);
	free(path);
	if (fd < 0)
	{
		report_error("heredoc_temp_open_failed", ERR_SYSCALL);
		return (open("/dev/null", O_RDONLY));
	}
	return (fd);
}
