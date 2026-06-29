/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/29 00:00:00 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

/* ── file parser ─────────────────────────────────────────────────────────── */
/* ./pipex infile cmd1 cmd2 ... outfile   (argc >= 5) */

static t_error	file_parser_parse(t_parser *this)
{
	if (this->argc < 5)
		return (set_error(ERR_INVALID_ARGS));
	this->result.input_type = TYPE_FILE;
	this->result.u_input.infile_name = this->argv[1];
	this->result.outfile_name = this->argv[this->argc - 1];
	this->result.command_num = this->argc - 3;
	this->result.pipe_num = this->result.command_num - 1;
	this->result.commands = &this->argv[2];
	return (set_error(ERR_OK));
}

static void	file_parser_destroy(t_parser *this)
{
	free(this);
}

static t_parser	*file_parser_create(int argc, char **argv)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (set_error(ERR_MALLOC_FAILED), NULL);
	parser->argc = argc;
	parser->argv = argv;
	parser->parse = file_parser_parse;
	parser->destroy = file_parser_destroy;
	return (parser);
}

/* ── heredoc parser ──────────────────────────────────────────────────────── */
/* ./pipex here_doc LIMITER cmd1 cmd2 ... outfile   (argc >= 6) */

static t_error	heredoc_parser_parse(t_parser *this)
{
	if (this->argc < 6)
		return (set_error(ERR_INVALID_ARGS));
	this->result.input_type = TYPE_HEREDOC;
	this->result.u_input.heredoc_end = this->argv[2];
	this->result.outfile_name = this->argv[this->argc - 1];
	this->result.command_num = this->argc - 4;
	this->result.pipe_num = this->result.command_num - 1;
	this->result.commands = &this->argv[3];
	return (set_error(ERR_OK));
}

static void	heredoc_parser_destroy(t_parser *this)
{
	free(this);
}

static t_parser	*heredoc_parser_create(int argc, char **argv)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (set_error(ERR_MALLOC_FAILED), NULL);
	parser->argc = argc;
	parser->argv = argv;
	parser->parse = heredoc_parser_parse;
	parser->destroy = heredoc_parser_destroy;
	return (parser);
}

/* ── factory ─────────────────────────────────────────────────────────────── */

t_parser	*parser_create(int argc, char **argv)
{
	if (argc >= 2 && strcmp(argv[1], "here_doc") == 0)
		return (heredoc_parser_create(argc, argv));
	return (file_parser_create(argc, argv));
}
