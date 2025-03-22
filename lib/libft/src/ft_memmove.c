/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:24:49 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/24 20:10:50 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*src_char;
	char		*dest_char;
	size_t		i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	src_char = (char *)src;
	dest_char = (char *)dest;
	if (dest_char > src_char)
		while (n-- > 0)
			dest_char[n] = src_char[n];
	else
	{
		while (i < n)
		{
			dest_char[i] = src_char[i];
			i++;
		}
	}
	return (dest);
}
