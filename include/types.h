/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:38:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/29 10:39:11 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef struct s_command		t_command;
typedef enum e_error			t_error;
typedef struct s_app			t_app;
typedef struct s_parser			t_parser;
typedef struct s_pipe			t_pipe;
typedef struct s_file_mgr		t_file_mgr;
typedef struct s_pipe_mgr		t_pipe_mgr;
typedef struct s_command_mgr	t_command_mgr;
typedef struct s_reader			t_reader;
typedef struct s_writer			t_writer;

/*
./pipex file1 cmd1 cmd2 file2
< file1 cmd1 | cmd2 > file2

./pipex here_doc LIMITER cmd1 cmd2 file
cmd1 << LIMITER | cmd2 >> file
*/

enum						e_input_type
{
	TYPE_FILE,
	TYPE_HEREDOC,
};

typedef struct s_parse_result
{
	enum e_input_type		input_type;
	union
	{
		char				*infile_name;
		char				*heredoc_end;
	} u_input;
	char					*outfile_name;
	int						command_num;
	int						pipe_num;
	char					**commands;
}							t_parse_result;

typedef struct s_parser
{
	t_parse_result			result;
	int						argc;
	char					**argv;

	t_error					(*parse)(t_parser *this);
	void					(*destroy)(t_parser *this);
}							t_parser;

typedef struct s_pipe_mgr
{
	int						pipe_num;
	int 					(*pipes)[2];

	void					(*connect)(t_pipe_mgr *this, int from_fd,
			int to_fd);
	void					(*close_other_pipes)(t_pipe_mgr *this);
	void					(*destroy)(t_pipe_mgr *this);
}							t_pipe_mgr;

typedef struct s_command_mgr
{
	int						command_num;
	char					**command;

	void					(*destroy)(t_command_mgr *this);
}							t_command_mgr;

// typedef struct s_file_mgr
// {
// 	char					(*read)(t_file_mgr *this);
// 	char					(*write)(t_file_mgr *this);
// 	void					(*destroy)(t_file_mgr *this);
// }							t_file_mgr;

typedef struct s_reader
{
	enum e_input_type		input_type;
	union
	{
		char				*infile_name;
		char				*heredoc_end;
	} u_input;

	char					(*read)(t_reader *this);
	void					(*destroy)(t_reader *this);
}							t_reader;

typedef struct s_writer
{
	char					(*write)(t_writer *this);
	void					(*destroy)(t_writer *this);
}							t_writer;

typedef struct s_app
{
	t_parser				*parser;
	t_command_mgr			cmd_mgr;
	t_pipe_mgr				pipe_mgr;

	t_error					(*run)(t_app *this);
	void					(*destroy)(t_app *this);
}							t_app;

#endif