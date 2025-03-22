/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:56:03 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/27 21:40:42 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	parse_conversions(va_list args, const char *format)
{
	int	chars;

	chars = 0;
	if (*format == 'c')
		chars += print_char(va_arg(args, int));
	else if (*format == 's')
		chars += print_string(va_arg(args, char *));
	else if (*format == 'p')
		chars += print_pointer(va_arg(args, void *));
	else if ((*format == 'd') || (*format == 'i'))
		chars += print_int(va_arg(args, int));
	else if (*format == 'u')
		chars += print_unsigned_int(va_arg(args, unsigned int));
	else if ((*format == 'x') || (*format == 'X'))
		chars += print_hex(va_arg(args, int), *format);
	else if (*format == '%')
	{
		ft_putchar_fd('%', 1);
		chars += 1;
	}
	return (chars);
}

int	ft_printf(const char *format, ...)
{
	va_list			args;
	unsigned int	chars;

	chars = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format != '%')
		{
			ft_putchar_fd(*format, 1);
			chars++;
			format++;
		}
		else
		{
			format++;
			chars += parse_conversions(args, format);
			format++;
		}
	}
	va_end(args);
	return (chars);
}
