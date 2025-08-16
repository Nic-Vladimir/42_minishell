/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 05:47:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:30:24 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

	len = ft_strlen(key) + ft_strlen(value) + 2;
	env_var = (char *)malloc(len);
	if (env_var)
	{
		ft_strlcpy(env_var, key, len);
		ft_strlcat(env_var, "=", len);
		ft_strlcat(env_var, value, len);
	}
	return (env_var);
}

static void	fill_envp_from_hashmap(char **envp, t_env *env)
{
	int			i;
	int			j;
	t_bucket	*current;

	i = 0;
	j = 0;
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
}

char	**get_envp_from_hashmap(t_env *env)
{
	char	**envp;
	int		count;

	count = get_vars_num(env);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	ft_memset(envp, 0, (count + 1) * sizeof(char *));
	fill_envp_from_hashmap(envp, env);
	return (envp);
}
