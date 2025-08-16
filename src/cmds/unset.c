/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:22:48 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:37:22 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	unset_env_var(t_env *env, char *key)
{
	int			index;
	t_bucket	*current_node;
	t_bucket	*prev_node;

	prev_node = NULL;
	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node)
	{
		if (ft_strcmp(current_node->key, key) == 0)
		{
			if (prev_node == NULL)
				env->vars->buckets[index] = current_node->next;
			else
				prev_node->next = current_node->next;
			free(current_node->key);
			free(current_node->value);
			free(current_node);
			return ;
		}
		prev_node = current_node;
		current_node = prev_node->next;
	}
}

int	execute_unset(t_env *env, t_ast_node *node)
{
	int	i;

	i = 1;
	if (!node->args[i])
	{
		ft_printf("unset: not enough arguments\n");
		return (1);
	}
	while (node->args[i])
	{
		unset_env_var(env, node->args[i++]);
	}
	return (0);
}
