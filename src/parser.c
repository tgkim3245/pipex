/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/30 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>

static void	parser_destroy(t_parser *this)
{
	free(this);
}

/* ./pipex infile cmd1 cmd2 ... outfile   (argc >= 5) */
static t_error	file_parser_parse(t_parser *this, t_parse_result *out)
{
	if (this->argc < 5)
		return (set_error(ERR_INVALID_ARGS));
	memset(out, 0, sizeof(*out));
	out->input_type = TYPE_FILE;
	out->input[TYPE_FILE] = this->argv[1];
	out->outfile_name = this->argv[this->argc - 1];
	out->command_num = this->argc - 3;
	out->pipe_num = out->command_num - 1;
	out->commands = &this->argv[2];
	return (set_error(ERR_OK));
}

/* ./pipex here_doc LIMITER cmd1 cmd2 ... outfile   (argc >= 6) */
static t_error	heredoc_parser_parse(t_parser *this, t_parse_result *out)
{
	if (this->argc < 6)
		return (set_error(ERR_INVALID_ARGS));
	memset(out, 0, sizeof(*out));
	out->input_type = TYPE_HEREDOC;
	out->input[TYPE_HEREDOC] = this->argv[2];
	out->outfile_name = this->argv[this->argc - 1];
	out->command_num = this->argc - 4;
	out->pipe_num = out->command_num - 1;
	out->commands = &this->argv[3];
	return (set_error(ERR_OK));
}

t_parser	*parser_create(int argc, char **argv)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (set_error(ERR_MALLOC_FAILED), NULL);
	parser->argc = argc;
	parser->argv = argv;
	parser->destroy = parser_destroy;
	if (argc >= 2 && strcmp(argv[1], "here_doc") == 0)
		parser->parse = heredoc_parser_parse;
	else
		parser->parse = file_parser_parse;
	return (parser);
}
