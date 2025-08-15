/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmap_insert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:26:39 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 14:26:47 by mgavorni         ###   ########.fr       */
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
	ft_memset(new_bucket, 0, sizeof(t_bucket));
	new_bucket->key = ft_strdup(key);
	new_bucket->value = ft_strdup(value);
	if (!new_bucket->key || !new_bucket->value)
	{
		free_bucket(new_bucket);
		return (1);
	}
	new_bucket->next = env->vars->buckets[index];
	env->vars->buckets[index] = new_bucket;
	return (0);
}
