/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 10:16:46 by taegokim          #+#    #+#             */
/*   Updated: 2026/06/23 11:36:30 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

int	main(int argc, char **argv)
{
	t_app	app;

	if (app_init(&app, argc, argv) != ERR_OK)
		return (print_error(), app.destroy(&app), 1);
	if (app.run(&app) != ERR_OK)
		return (print_error(), app.destroy(&app), 1);
	app.destroy(&app);
	return (0);
}
