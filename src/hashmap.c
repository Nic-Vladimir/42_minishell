/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 05:47:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/21 17:04:46 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	};
	return (hash);
}

int	hashmap_insert(t_env *env, char *key, char *value)
{
	int			index;
	t_bucket	*current_node;
	t_bucket	*new_node;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node)
	{
		if (ft_strcmp(current_node->key, key) == 0)
		{
			free(current_node->value);
			current_node->value = ft_strdup(value);
			return (0);
		}
		current_node = current_node->next;
	}
	new_node = (t_bucket *)malloc(sizeof(t_bucket));
	if (!new_node)
		return (1);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (1);
	}
	new_node->next = env->vars->buckets[index];
	env->vars->buckets[index] = new_node;
	return (0);
}

int	get_vars_num(t_env *env)
{
	t_bucket	*current;
	int			count;
	int			j;

	j = 0;
	count = 0;
	while (j < env->vars->size)
	{
		current = env->vars->buckets[j];
		while (current)
		{
			count++;
			current = current->next;
		}
		j++;
	}
	return (count);
}

char	**get_envp_from_hashmap(t_env *env)
{
	char		**envp;
	int			i;
	int			j;
	int			count;
	t_bucket	*current;
	char		*env_var;

	i = 0;
	j = 0;
	count = get_vars_num(env);
	envp = malloc((count + 1) * sizeof(char *));
	while (j < env->vars->size)
	{
		current = env->vars->buckets[j];
		while (current)
		{
			env_var = (char *)malloc(strlen(current->key)
					+ strlen(current->value) + 2);
			sprintf(env_var, "%s=%s", current->key, current->value);
			envp[i++] = env_var;
			current = current->next;
		}
		j++;
	}
	envp[i] = NULL;
	return (envp);
}
