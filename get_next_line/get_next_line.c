/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:05:35 by taegokim          #+#    #+#             */
/*   Updated: 2026/05/02 13:02:40 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_to_leftover(int fd, char *leftover)
{
	char	*buf;
	ssize_t	ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	while (!gnl_strchr(leftover, '\n'))
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret < 0)
			return (free(buf), free(leftover), NULL);
		if (ret == 0)
			break ;
		buf[ret] = '\0';
		leftover = gnl_strjoin(leftover, buf);
		if (!leftover)
			return (free(buf), NULL);
	}
	return (free(buf), leftover);
}

static char	*extract_line(char *leftover)
{
	char	*newline;
	char	*line;
	size_t	len;
	size_t	i;

	if (!leftover)
		return (NULL);
	newline = gnl_strchr(leftover, '\n');
	if (newline)
		len = (size_t)(newline - leftover + 1);
	else
		len = gnl_strlen(leftover);
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = leftover[i];
		i++;
	}
	line[len] = '\0';
	return (line);
}

static char	*trim_leftover(char *leftover)
{
	char	*newline;
	char	*trimmed;
	size_t	i;

	newline = gnl_strchr(leftover, '\n');
	if (!newline)
		return (free(leftover), NULL);
	newline++;
	trimmed = malloc(gnl_strlen(newline) + 1);
	if (!trimmed)
		return (free(leftover), NULL);
	i = 0;
	while (newline[i])
	{
		trimmed[i] = newline[i];
		i++;
	}
	trimmed[i] = '\0';
	return (free(leftover), trimmed);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	leftover = read_to_leftover(fd, leftover);
	if (!leftover || leftover[0] == '\0')
		return (free(leftover), leftover = NULL, NULL);
	line = extract_line(leftover);
	leftover = trim_leftover(leftover);
	return (line);
}
