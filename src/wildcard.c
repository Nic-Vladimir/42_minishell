/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:21:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/25 22:15:37 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <dirent.h>

static int match_pattern(const char *name, const char *prefix, const char *suffix)
{
    int name_len = ft_strlen(name);
    int prefix_len = ft_strlen(prefix);
    int suffix_len = ft_strlen(suffix);

    if (name_len < prefix_len + suffix_len)
        return (0);
    if (prefix_len > 0 && ft_strncmp(name, prefix, prefix_len) != 0)
        return (0);
    if (suffix_len > 0 && ft_strcmp(name + name_len - suffix_len, suffix) != 0)
        return (0);
    return (1);
}

char *expand_wildcard(char *arg)
{
    char			*prefix;
    char			*suffix;
    char			*result;
    DIR				*dir;
    struct dirent	*entry;
    int				match_count;

    // Extract prefix and suffix around first '*'
    prefix = ft_substr(arg, 0, ft_strchr(arg, '*') - arg);
    suffix = ft_strdup(ft_strchr(arg, '*') + 1);
    result = ft_strdup("");
    match_count = 0;

    // Open current directory
    dir = opendir(".");
    if (!dir)
    {
        free(prefix);
        free(suffix);
		free(result);
        arg = ft_strdup(arg); // Fallback to original
        return (ft_strdup(arg));
    }

    // Collect matching files
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.' && match_pattern(entry->d_name, prefix, suffix))
        {
			if (match_count > 0)
				result = ft_strjoin_free(result, " ");
			result = ft_strjoin_free(result, entry->d_name);
			match_count++;
        }
    }
    closedir(dir);

    // Replace args with matches
    if (match_count == 0)
	{
        free(result);
		result = ft_strdup(arg);
	}

    free(prefix);
    free(suffix);
    return (result);
}
