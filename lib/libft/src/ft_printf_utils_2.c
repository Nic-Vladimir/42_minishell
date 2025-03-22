/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:11:57 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/27 21:40:38 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	count_chars(char *buffer)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (buffer[i])
	{
		count++;
		i++;
	}
	return (count);
}

int	print_hex(unsigned int num, char format)
{
	char		*digits;
	size_t		len;

	len = 0;
	digits = "0123456789abcdef";
	if (format == 'X')
		digits = "0123456789ABCDEF";
	if (num >= 16)
		len += print_hex(num / 16, format);
	return (len += print_char(digits[num % 16]));
}

int	get_pointer_hex(unsigned long num)
{
	size_t	chars;
	char	*digits;

	chars = 0;
	digits = "0123456789abcdef";
	if (num >= 16)
		chars += get_pointer_hex(num / 16);
	return (chars += print_char(digits[num % 16]));
}

int	print_int(int num)
{
	unsigned int	len;

	len = count_digits(num);
	ft_putnbr_fd(num, 1);
	return (len);
}
