/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:36:15 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 15:55:50 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "error.h"

typedef enum e_input_type
{
	TYPE_FILE,
	TYPE_HEREDOC,
	TYPE_COUNT,
}					t_input_type;

typedef struct s_parsed
{
	t_input_type	input_type;
	char			*input[TYPE_COUNT];
	char			*outfile_name;
	int				command_num;
	char			**commands;
}					t_parsed;

struct				s_parser
{
	t_parsed		parsed;

	int				argc;
	char			**argv;

	t_status		(*parse)(t_parser * this);
	void			(*destroy)(t_parser *this);
};

t_status			parser_init(t_parser *this, int argc, char **argv);

#endif
