/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:12:03 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/25 14:31:34 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "command.h"
# include "error.h"
# include "types.h"


t_error			app_init(t_app *this, int argc, char **argv);

#endif