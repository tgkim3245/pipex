/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:38:18 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/09 12:42:03 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef struct s_parse_result	t_parse_result;
typedef struct s_parser			t_parser;
typedef struct s_cmd			t_cmd;
typedef struct s_cmd_mgr		t_cmd_mgr;
typedef struct s_pipe_mgr		t_pipe_mgr;
typedef struct s_fd_factory		t_fd_factory;
typedef struct s_app			t_app;

typedef enum e_status
{
	OK,
	FAIL,
}								t_status;

#endif
