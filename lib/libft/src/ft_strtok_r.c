/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:54:41 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:08:51 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*start;

	if (!str && (!saveptr || !*saveptr))
		return (NULL);
	if (!str)
		str = *saveptr;
	while (*str && ft_strchr(delim, *str))
		str++;
	if (!*str)
		return (*saveptr = NULL, NULL);
	start = str;
	while (*str && !ft_strchr(delim, *str))
		str++;
	if (*str)
	{
		*str = '\0';
		*saveptr = str + 1;
	}
	else
		*saveptr = NULL;
	return (start);
}
