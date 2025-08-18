/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:06:34 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/18 11:19:10 by mgavorni         ###   ########.fr       */
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
	ft_memset(tok_data, 0, sizeof(t_tokenizer_data));
	return (tok_data);
}

static void	insert_env_var(t_env *env, char *envp_entry)
{
	char	*key;
	char	*value;
	char	**res;

	res = ft_split(envp_entry, '=');
	if (!res || !res[0] || !res[1])
	{
		if (res)
			free_str_array(res);
		return ;
	}
	key = ft_strdup(res[0]);
	value = ft_strdup(res[1]);
	if (key && value)
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
	ft_memset(vars, 0, sizeof(t_hashmap));
	vars->size = 100;
	vars->buckets = (t_bucket **)malloc(vars->size * sizeof(t_bucket *));
	if (vars->buckets == NULL)
		return (NULL);
	ft_memset(vars->buckets, 0, vars->size * sizeof(t_bucket *));
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
	env->shell_pid = ft_getpid();
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
	env->pipeline = NULL;
	return (env);
}
