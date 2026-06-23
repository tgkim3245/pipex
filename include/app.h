/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:12:03 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 12:40:04 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "types.h"
# include "error.h"
# include "cmd.h"

typedef struct s_app
{
	char	*infile_name;
	char	*outfile_name;
	int		cmd_num;
	t_cmd	*cmd;

	t_error	(*run)(t_app * this);
	void	(*destroy)(t_app *this);
}			t_app;

t_error		app_init(t_app *this, int argc, char **argv);

#endif