/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:30:06 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/24 20:10:50 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int	int_len(long number);
static char	*pre_conv(int len);

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	number;

	number = n;
	len = int_len(number);
	result = pre_conv(len);
	if (!result)
		return (NULL);
	if (number < 0)
		number = -number;
	i = len - 1;
	while (number != 0)
	{
		result[i] = ((number % 10) + 48);
		number = number / 10;
		i--;
	}
	if (n < 0)
		result[0] = '-';
	result[len] = '\0';
	return (result);
}

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	tmp[0] = '0';
	return (tmp);
}

static int	int_len(long number)
{
	int	count;

	count = 0;
	if (number < 0)
	{
		count++;
		number = -number;
	}
	if (number == 0)
		count++;
	while (number != 0)
	{
		number /= 10;
		count++;
	}
	return (count);
}
