/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 11:47:19 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/30 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "error.h"

/*
** minishell 확장 시 redirections 필드를 여기에 추가
*/
struct s_command
{
	char	**args;
	char	**envp;
	t_error	(*run)(t_command *this, int in_fd, int out_fd);
	void	(*destroy)(t_command *this);
};

struct s_command_mgr
{
	int			command_num;
	t_command	*commands;
	void		(*destroy)(t_command_mgr *this);
};

t_command	*command_create(char *cmd_str, char **envp);
t_error		command_mgr_init(t_command_mgr *this, int command_num,
				char **raw_commands, char **envp);

#endif
