/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:06:34 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/03 18:18:23 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static t_tokenizer_data	*init_tok_data(void)
{
	t_tokenizer_data	*tok_data;

	tok_data = (t_tokenizer_data *)malloc(sizeof(t_tokenizer_data));
	if (!tok_data)
		return (NULL);
	tok_data->tokens = NULL;
	tok_data->tail = NULL;
	return (tok_data);
}

static void	insert_env_var(t_env *env, char *envp_entry)
{
	char	**res;
	char	*key;
	char	*value;

	res = ft_split(envp_entry, '=');
	if (!res || !res[0] || !res[1])
	{
		if (res)
			free(res);
		return ;
	}
	key = malloc(strlen(res[0]) + 1);
	value = malloc(strlen(res[1]) + 1);
	strcpy(key, res[0]);
	strcpy(value, res[1]);
	hashmap_insert(env, key, value);
	free(key);
	free(value);
	free_str_array(res);
}

static t_hashmap	*init_hashmap_vars(void)
{
	t_hashmap	*vars;
	int			i;

	i = 0;
	vars = (t_hashmap *)malloc(sizeof(t_hashmap));
	if (!vars)
		return (NULL);
	vars->size = 100;
	vars->buckets = (t_bucket **)malloc(vars->size * sizeof(t_bucket *));
	if (vars->buckets == NULL)
		return (NULL);
	while (i < vars->size)
		vars->buckets[i++] = NULL;
	return (vars);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;

	i = 0;
	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	ft_memset(env, 0, sizeof(t_env));
	sig_malinit(&env->sigenv);
	if (!env->sigenv)
		return (NULL);
	env->shell_pid = getpid();
	env->last_exit_code = 0;
	env->vars = init_hashmap_vars();
	if (!env->vars)
		return (NULL);
	env->vars->size = 100;
	i = 0;
	while (envp[i])
		insert_env_var(env, envp[i++]);
	env->tokenizer = init_tok_data();
	env->root = NULL;
	return (env);
}
