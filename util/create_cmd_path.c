/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:19:02 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 17:11:26 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "libft.h"
#include <unistd.h>

static char	*get_path_env(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	return (NULL);
}

static char	*find_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*dir_slash;
	char	*path;
	int		i;

	dirs = ft_split(get_path_env(envp), ':');
	if (!dirs)
		return (NULL);
	i = -1;
	while (dirs[++i])
	{
		dir_slash = ft_strjoin(dirs[i], "/");
		if (!dir_slash)
			break ;
		path = ft_strjoin(dir_slash, cmd);
		free(dir_slash);
		if (!path)
			break ;
		if (access(path, X_OK) == 0)
			return (free_split(dirs), path);
		free(path);
	}
	return (free_split(dirs), NULL);
}

char	*create_cmd_path(char *cmd_name, char **envp)
{
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	return (find_cmd_path(cmd_name, envp));
}
