/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:21:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 15:36:45 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	match_pattern(const char *name, const char *prefix,
		const char *suffix)
{
	int	name_len;
	int	prefix_len;
	int	suffix_len;

	name_len = ft_strlen(name);
	prefix_len = ft_strlen(prefix);
	suffix_len = ft_strlen(suffix);
	if (name_len < prefix_len + suffix_len)
		return (0);
	if (prefix_len > 0 && ft_strncmp(name, prefix, prefix_len) != 0)
		return (0);
	if (suffix_len > 0 && ft_strcmp(name + name_len - suffix_len, suffix) != 0)
		return (0);
	return (1);
}

static char	*process_matching_entries(DIR *dir, char *prefix, char *suffix,
		char *result)
{
	struct dirent	*entry;
	int				match_count;

	match_count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && match_pattern(entry->d_name, prefix,
				suffix))
		{
			if (match_count > 0)
				result = ft_strjoin_free(result, " ");
			result = ft_strjoin_free(result, entry->d_name);
			match_count++;
		}
		entry = readdir(dir);
	}
	return (result);
}

static char	*handle_no_matches(char *result, char *arg)
{
	free(result);
	return (ft_strdup(arg));
}

static char	*handle_wildcard_matches(char *prefix, char *suffix, char *arg)
{
	DIR		*dir;
	char	*result;

	result = ft_strdup("");
	dir = opendir(".");
	if (!dir)
	{
		free(prefix);
		free(suffix);
		free(result);
		return (ft_strdup(arg));
	}
	result = process_matching_entries(dir, prefix, suffix, result);
	closedir(dir);
	if (ft_strlen(result) == 0)
		result = handle_no_matches(result, arg);
	return (result);
}

char	*expand_wildcard(char *arg)
{
	char	*prefix;
	char	*result;
	char	*suffix;

	prefix = ft_substr(arg, 0, ft_strchr(arg, '*') - arg);
	suffix = ft_strdup(ft_strchr(arg, '*') + 1);
	result = handle_wildcard_matches(prefix, suffix, arg);
	free(prefix);
	free(suffix);
	return (result);
}
