/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 05:47:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/03 18:16:42 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	update_value_if_key_exists(t_bucket *current_bucket, char *key,
		char *value)
{
	while (current_bucket)
	{
		if (ft_strcmp(current_bucket->key, key) == 0)
		{
			free(current_bucket->value);
			current_bucket->value = ft_strdup(value);
			return (true);
		}
		current_bucket = current_bucket->next;
	}
	return (false);
}

int	hashmap_insert(t_env *env, char *key, char *value)
{
	int			index;
	t_bucket	*current_bucket;
	t_bucket	*new_bucket;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_bucket = env->vars->buckets[index];
	if (update_value_if_key_exists(current_bucket, key, value))
		return (0);
	new_bucket = (t_bucket *)malloc(sizeof(t_bucket));
	if (!new_bucket)
		return (1);
	new_bucket->key = ft_strdup(key);
	new_bucket->value = ft_strdup(value);
	if (!new_bucket->key || !new_bucket->value)
	{
		free(new_bucket->key);
		free(new_bucket->value);
		free(new_bucket);
		return (1);
	}
	new_bucket->next = env->vars->buckets[index];
	env->vars->buckets[index] = new_bucket;
	return (0);
}

static int	get_vars_num(t_env *env)
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

static char	*get_env_var_string(char *key, char *value)
{
	char	*env_var;
	size_t	len;

	len = strlen(key) + strlen(value) + 2;
	env_var = (char *)malloc(len);
	if (env_var)
		sprintf(env_var, "%s=%s", key, value);
	return (env_var);
}

char	**get_envp_from_hashmap(t_env *env)
{
	char		**envp;
	int			i;
	int			j;
	int			count;
	t_bucket	*current;

	i = 0;
	j = 0;
	count = get_vars_num(env);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	while (j < env->vars->size)
	{
		current = env->vars->buckets[j];
		while (current)
		{
			envp[i] = get_env_var_string(current->key, current->value);
			current = current->next;
			i++;
		}
		j++;
	}
	envp[i] = NULL;
	return (envp);
}

// char	**get_envp_from_hashmap(t_env *env)
//{
//	char		**envp;
//	int			i;
//	int			j;
//	int			count;
//	t_bucket	*current;
//	char		*env_var;
//
//	i = 0;
//	j = 0;
//	count = get_vars_num(env);
//	envp = malloc((count + 1) * sizeof(char *));
//	while (j < env->vars->size)
//	{
//		current = env->vars->buckets[j];
//		while (current)
//		{
//			env_var = (char *)malloc(strlen(current->key)
//					+ strlen(current->value) + 2);
//			sprintf(env_var, "%s=%s", current->key, current->value);
//			envp[i++] = env_var;
//			current = current->next;
//		}
//		j++;
//	}
//	envp[i] = NULL;
//	return (envp);
//}
