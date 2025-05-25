/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:51:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/25 16:37:38 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"



void	free_str_array(char **arr)
{
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++)
		free(arr[i]);
	free(arr);
}

static t_tokenizer_data	*init_tok_data(void)
{
	t_tokenizer_data	*tok_data;

	tok_data = (t_tokenizer_data *)malloc(sizeof(t_tokenizer_data));
	if (!tok_data)
		return (NULL);
	ft_memset(tok_data, 0, sizeof(t_tokenizer_data));
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
	if(!env->sigenv)
		return(NULL);
	env->shell_pid = getpid();
	env->last_exit_code = 0;
	env->vars = (t_hashmap *)malloc(sizeof(t_hashmap));
	if (!env->vars)
		return (NULL);
	ft_memset(env->vars, 0, sizeof(t_hashmap));
	env->vars->size = 100;
	env->vars->buckets = (t_bucket **)malloc(env->vars->size
			* sizeof(t_bucket *));
	if (env->vars->buckets == NULL)
		return (NULL);
	ft_memset(env->vars->buckets, 0, env->vars->size * sizeof(t_bucket *));
	while (i < env->vars->size)
		env->vars->buckets[i++] = NULL;
	i = 0;
	while (envp[i])
		insert_env_var(env, envp[i++]);
	env->tokenizer = init_tok_data();
	env->root = NULL;
	return (env);
}

char	*get_env_value(t_env *env, const char *key)
{
	int			index;
	t_bucket	*current_node;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node)
	{
		if (strcmp(current_node->key, key) == 0)
		{
			return (current_node->value);
		}
		current_node = current_node->next;
	}
	return (NULL);
}
