/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matus <matus@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:38:53 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 17:11:49 by matus            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

// 

// static int	ft_word_count(const char *str, char c);
// static char	*fill_word(const char *str, int start, int end);
// static void	*ft_free(char **strs, int count);
// static void	ft_initiate_vars(size_t *i, int *j, int *s_word);

// char	**ft_split(const char *s, char c)
// {
// 	char	**res;
// 	size_t	i;
// 	int		j;
// 	int		s_word;

// 	ft_initiate_vars(&i, &j, &s_word);
// 	res = ft_calloc ((ft_word_count(s, c) + 1), sizeof (char *));
// 	if (!res)
// 		return (NULL);
// 	while (i <= ft_strlen(s))
// 	{
// 		if (s[i] != c && s_word < 0)
// 			s_word = i;
// 		else if ((s[i] == c || i == ft_strlen(s)) && s_word >= 0)
// 		{
// 			res[j] = fill_word(s, s_word, i);
// 			if (!(res[j]))
// 				return (ft_free(res, j));
// 			s_word = -1;
// 			j++;
// 		}
// 		i++;
// 	}
// 	return (res);
// }

// static void	ft_initiate_vars(size_t *i, int *j, int *s_word)
// {
// 	*i = 0;
// 	*j = 0;
// 	*s_word = -1;
// }

// static void	*ft_free(char **strs, int count)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		free(strs[i]);
// 		i++;
// 	}
// 	free(strs);
// 	return (NULL);
// }

// static char	*fill_word(const char *str, int start, int end)
// {
// 	char	*word;
// 	int		i;

// 	i = 0;
// 	word = malloc((end - start + 1) * sizeof(char));
// 	if (!word)
// 		return (NULL);
// 	while (start < end)
// 	{
// 		word[i] = str[start];
// 		i++;
// 		start++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// static int	ft_word_count(const char *str, char c)
// {
// 	int	count;
// 	int	trigger;

// 	count = 0;
// 	trigger = 0;
// 	while (*str)
// 	{
// 		if (*str != c && trigger == 0)
// 		{
// 			trigger = 1;
// 			count++;
// 		}
// 		else if (*str == c)
// 			trigger = 0;
// 		str++;
// 	}
// 	return (count);
// }

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