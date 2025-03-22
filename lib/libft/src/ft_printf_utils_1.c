/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 19:32:57 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/27 21:40:40 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	count_digits(int num)
{
	int	count;
	int	copy;

	copy = num;
	count = 0;
	if (copy < 0)
		count++;
	if (copy == 0)
		return (1);
	while (copy != 0)
	{
		copy /= 10;
		count++;
	}
	return (count);
}

int	print_char(int character)
{
	ft_putchar_fd(character, 1);
	return (1);
}

int	print_string(char *str)
{
	if (str == NULL)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}

int	print_pointer(void *ptr)
{
	size_t	chars;

	if (ptr == NULL)
		return (write(1, "(nil)", 5), 5);
	write(1, "0x", 2);
	chars = 2;
	chars += get_pointer_hex((unsigned long)ptr);
	return (chars);
}

int	print_unsigned_int(unsigned int num)
{
	size_t	chars;

	chars = 0;
	if (num >= 10)
		chars += print_unsigned_int(num / 10);
	return (chars += print_char(num % 10 + '0'));
}
