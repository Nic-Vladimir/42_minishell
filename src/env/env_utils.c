/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:02:41 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 11:27:03 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

static char	*check_executable_path(const char *path)
{
	char	*result;

	result = NULL;
	if (access(path, X_OK) == 0)
		result = strdup(path);
	return (result);
}

static char	*search_in_directory(const char *dir, const char *command)
{
	char	*full_path;
	size_t	path_len;
	char	*result;

	path_len = strlen(dir) + strlen(command) + 2;
	full_path = malloc(path_len);
	if (!full_path)
		return (NULL);
	ft_memset(full_path, 0, path_len);
	snprintf(full_path, path_len, "%s/%s", dir, command);
	result = check_executable_path(full_path);
	free(full_path);
	return (result);
}

char	*find_executable(t_env *env, const char *command)
{
	char	*path_env;
	char	*path_copy;
	char	*dir;
	char	*result;
	char	*saveptr;

	result = NULL;
	fprintf(stderr, "[DEBUG] find_executable: %s\n", command);
	if (!command)
	{
		return (result);
	}
	if (ft_strchr(command, '/'))
		return (check_executable_path(command));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);
	dir = strtok_r(path_copy, ":", &saveptr);
	while (dir && !result)
	{
		result = search_in_directory(dir, command);
		dir = strtok_r(NULL, ":", &saveptr);
	}
	free(path_copy);
	return (result);
}

char	*get_env_value(t_env *env, const char *key)
{
	int			index;
	t_bucket	*current_node;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node)
	{
		if (strcmp(current_node->key, key) == 0)
		{
			return (current_node->value);
		}
		current_node = current_node->next;
	}
	return (NULL);
}

int	djb2_hash(const char *key)
{
	size_t	i;
	int		hash;

	i = 0;
	hash = 5381;
	while (i < ft_strlen(key))
	{
		hash = ((hash << 5) + hash) + key[i];
		i++;
	}
	return (hash);
}

// char	*find_executable(t_env *env, const char *command)
//{
//	char	*path_env;
//	char	*path_copy;
//	char	*dir;
//	char	*path_ptr;
//	char	*saveptr;
//	char	*full_path;
//	size_t	path_len;
//
//	if (ft_strchr(command, '/'))
//		return (strdup(command));
//	// Get PATH from environment
//	path_env = get_env_value(env, "PATH");
//	if (!path_env)
//		return (NULL);
//	path_copy = strdup(path_env);
//	if (!path_copy)
//		return (NULL);
//	// Tokenize PATH by ':'
//	path_ptr = path_copy;
//	while ((dir = strtok_r(path_ptr, ":", &saveptr)) != NULL)
//	{
//		path_ptr = NULL; // For subsequent calls
//		// Construct full path: dir + '/' + command
//		path_len = strlen(dir) + strlen(command) + 2;
//		// +2 for '/' and null terminator
//		full_path = malloc(path_len);
//		if (!full_path)
//			continue ;
//		snprintf(full_path, path_len, "%s/%s", dir, command);
//		// Check if file exists and is executable
//		if (access(full_path, X_OK) == 0)
//		{
//			free(path_copy);
//			return (full_path);
//		}
//		free(full_path);
//	}
//	free(path_copy);
//	return (NULL);
//}
