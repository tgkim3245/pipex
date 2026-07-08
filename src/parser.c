/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/07 17:16:39 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

static void	parser_destroy(t_parser *this)
{
	(void)this;
}

/* ./pipex infile cmd1 cmd2 ... outfile   (argc >= 5) */
static t_status	file_parser_parse(t_parser *this)
{
	t_parsed	*parsed;

	parsed = &this->parsed;
	if (this->argc < 5)
		return (report_error("parser", ERR_INVALID_ARGS));
	parsed->input_type = TYPE_FILE;
	parsed->input = this->argv[1];
	parsed->outfile_name = this->argv[this->argc - 1];
	parsed->command_num = this->argc - 3;
	parsed->commands = &this->argv[2];
	return (OK);
}

/* ./pipex here_doc LIMITER cmd1 cmd2 ... outfile   (argc >= 6) */
static t_status	heredoc_parser_parse(t_parser *this)
{
	t_parsed	*parsed;

	parsed = &this->parsed;
	if (this->argc < 6)
		return (report_error("parser", ERR_INVALID_ARGS));
	parsed->input_type = TYPE_HEREDOC;
	parsed->input = this->argv[2];
	parsed->outfile_name = this->argv[this->argc - 1];
	parsed->command_num = this->argc - 4;
	parsed->commands = &this->argv[3];
	return (OK);
}

t_status	parser_init(t_parser *this, int _argc, char **_argv)
{
	ft_memset(&this->parsed, 0, sizeof(t_parsed));
	this->argc = _argc;
	this->argv = _argv;
	this->destroy = parser_destroy;
	if (this->argc >= 2 && ft_strncmp(this->argv[1], "here_doc", 9) == 0)
		this->parse = heredoc_parser_parse;
	else
		this->parse = file_parser_parse;
	return (OK);
}
