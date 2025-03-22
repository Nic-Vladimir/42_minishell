/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 05:47:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/18 22:38:48 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int djb2_hash(const char *key) {
	size_t	i;
	int		hash;

	i = 0;
    hash = 5381;
	while (i < ft_strlen(key)) {
		hash = ((hash << 5) + hash) + key[i];
		i++;
	};
    return hash;
}

int hashmap_insert(t_env *env, char *key, char *value) {
	int			index;
	t_bucket	*current_node;
	t_bucket	*new_node;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node) {
		if (strcmp(current_node->key, key) == 0) {
			current_node->value = (char *)arena_malloc(env->arena, ft_strlen(value) + 1);
			strcpy(current_node->value, value);
            return 0;
		}
		current_node = current_node->next;
	}
	new_node = (t_bucket *)arena_malloc(env->arena, sizeof(t_bucket));
	new_node->key = (char *)arena_malloc(env->arena, ft_strlen(key) + 1);
	new_node->value = (char *)arena_malloc(env->arena, ft_strlen(value) + 1);
	strcpy(new_node->key, key);
	strcpy(new_node->value, value);
	new_node->next = env->vars->buckets[index];
	env->vars->buckets[index] = new_node;
	return 1;
}

int get_vars_num(t_env *env) {
    t_bucket	*current;
	int			count;
	int			j;

	j = 0;
	while (j < env->vars->size) {
        current = env->vars->buckets[j];
        while (current) {
            count++;
            current = current->next;
        }
		j++;
    }
	return count;
}

char **get_envp_from_hashmap(t_env *env) {
    char		**envp;
    int			i;
	int			j;
    int			count;
	t_bucket	*current;

	i = 0;
	j = 0;
	count = get_vars_num(env);
    envp = arena_malloc(env->arena, (count + 1) * sizeof(char *));
	while (j < env->vars->size) {
        current = env->vars->buckets[j];
        while (current) {
            char *env_var = (char *)arena_malloc(env->arena, strlen(current->key) + strlen(current->value) + 2);
            sprintf(env_var, "%s=%s", current->key, current->value);
            envp[i++] = env_var;
            current = current->next;
        }
		j++;
    }
    envp[i] = NULL;
    return envp;
}



