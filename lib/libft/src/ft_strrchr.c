/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:01:38 by vnicoles          #+#    #+#             */
/*   Updated: 2024/12/24 20:10:50 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;
	char			*res;
	char			cc;

	cc = (char) c;
	res = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == cc)
			res = (char *) &s[i];
		i++;
	}
	if (s[i] == cc)
		res = (char *) &s[i];
	return (res);
}
