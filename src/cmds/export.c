/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:17:49 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:25:21 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	print_sorted_envp(t_env *env)
{
	int		i;
	char	**envp;
	ssize_t	write_status;

	i = 0;
	envp = get_envp_from_hashmap(env);
	while (envp[i])
		i++;
	sort_envp(envp, 0, i - 1);
	format_envp(envp);
	i = 0;
	while (envp[i])
	{
		write_status = write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
		if (write_status == -1 || write(STDOUT_FILENO, "\n", 1) == -1)
		{
			free_envp(envp);
			return (1);
		}
		i++;
	}
	free_envp(envp);
	return (0);
}

static int	extract_key_value(char *temp, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(temp, '=');
	if (equal)
	{
		*key = ft_substr(temp, 0, equal - temp);
		*value = ft_substr(temp, equal - temp + 1, ft_strlen(equal) + 1);
	}
	else
	{
		*key = ft_strdup(temp);
		*value = ft_strdup("");
	}
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		*key = NULL;
		*value = NULL;
		return (-1);
	}
	return (0);
}

int	execute_export(t_env *env, t_ast_node *node)
{
	char	*temp;
	char	*key;
	char	*value;
	int		i;

	i = 1;
	if (!node->args[i])
		return (print_sorted_envp(env));
	while (node->args[i])
	{
		temp = handle_quotes(node, &i);
		i++;
		if (extract_key_value(temp, &key, &value) != 0)
			return (1);
		if (hashmap_insert(env, key, value) != 0)
			return (1);
		free(key);
		free(value);
		free(temp);
	}
	return (0);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
