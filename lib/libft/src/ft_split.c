/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:38:53 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/28 14:07:38 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static size_t	counter(char const *s, char c)
{
	size_t	count;

	count = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static size_t	ft_word_len(char const *s, char c)
{
	if (!s)
		return (0);
	if (!(ft_strchr(s, c)))
		return (ft_strlen(s));
	else
		return (ft_strchr(s, c) - s);
}

char	**ft_split(char const *s, char c)
{
	char	**new_str;
	size_t	i;
	size_t	word_length;

	new_str = (char **)malloc((sizeof(char *)) * (counter(s, c) + 1));
	if (!s || !new_str)
		return (0);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			word_length = ft_word_len(s, c);
			new_str[i++] = ft_substr(s, 0, word_length);
			s += word_length;
		}
	}
	new_str[i] = NULL;
	return (new_str);
}
