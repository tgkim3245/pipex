/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_mgr.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 00:00:00 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/05 16:46:45 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_MGR_H
# define PIPE_MGR_H

# include "error.h"

struct s_pipe_mgr
{
	int		pipe_num;
	int		(*pipes)[2];

	void	(*close_other_pipes)(t_pipe_mgr *this);
	void	(*destroy)(t_pipe_mgr *this);
};

t_status	pipe_mgr_init(t_pipe_mgr *this, int pipe_num);

#endif
