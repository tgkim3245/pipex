/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taegokim <taegokim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 16:55:36 by taegokim          #+#    #+#             */
/*   Updated: 2026/04/27 16:18:21 by taegokim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdint.h>

typedef int		(*t_print_format)(va_list);

/* src */
int				ft_printf(const char *str, ...);
int				print_format_c(va_list args);
int				print_format_s(va_list args);
int				print_format_p(va_list args);
int				print_format_d(va_list args);
int				print_format_i(va_list args);
int				print_format_u(va_list args);
int				print_format_x(va_list args);
int				print_format_upper_x(va_list args);
int				print_format_percent(va_list args);

/* util */
unsigned int	cnt_base_x(long long n, unsigned int base);
int				nbr2base_str(long long nbr, char **hex, unsigned int base);
unsigned int	cnt_ptr_base_x(uintptr_t n, unsigned int base);
int				ptr2base_str(uintptr_t nbr, char **str, unsigned int base);

#endif