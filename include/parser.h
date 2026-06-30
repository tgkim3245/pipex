/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:36:15 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/30 00:00:00 by taegokim         ###   ########.fr       */
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
}	t_input_type;

struct s_parse_result
{
	t_input_type	input_type;
	char			*input[TYPE_COUNT];
	char			*outfile_name;
	int				command_num;
	int				pipe_num;
	char			**commands;
};

struct s_parser
{
	int		argc;
	char	**argv;
	t_error	(*parse)(t_parser *this, t_parse_result *out);
	void	(*destroy)(t_parser *this);
};

t_parser	*parser_create(int argc, char **argv);

#endif
