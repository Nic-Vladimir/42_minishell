/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:02:41 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/21 16:29:23 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	if (ft_strchr(command, '/'))
		return (check_executable_path(command));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);
	result = NULL;
	dir = strtok_r(path_copy, ":", &saveptr);
	while (dir && !result)
	{
		result = search_in_directory(dir, command);
		dir = strtok_r(NULL, ":", &saveptr);
	}
	free(path_copy);
	return (result);
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
