/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:16:46 by taegokim          #+#    #+#             */
/*   Updated: 2026/07/06 10:07:24 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_app	app;

	ft_memset(&app, 0, sizeof(t_app));
	if (app_init(&app, argc, argv, envp) != OK)
		return (app.destroy(&app), 1);
	if (app.run(&app) != OK)
		return (app.destroy(&app), 1);
	app.destroy(&app);
	return (app.cmd_mgr.exit_code);
}
