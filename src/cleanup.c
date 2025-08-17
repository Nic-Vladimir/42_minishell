/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 21:35:07 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/17 21:01:36 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/env.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

void	free_tokens(t_tokenizer_data *tokenizer)
{
	t_token	*current;
	t_token	*next;

	current = tokenizer->tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	tokenizer->tokens = NULL;
	tokenizer->tail = NULL;
}

void	free_ast(t_ast_node **node_ptr)
{
	int			i;
	t_ast_node	*node;

	if (!node_ptr || !*node_ptr)
		return ;
	node = *node_ptr;
	if (node->left)
		free_ast(&(node->left));
	if (node->right)
		free_ast(&(node->right));
	if (node->group)
		free_ast(&(node->group));
	if (node->args)
	{
		i = 0;
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	if (node->arg_types)
		free(node->arg_types);
	free(node);
	node = NULL;
	*node_ptr = NULL;
}

static void	free_hashmap(t_hashmap *hashmap)
{
	t_bucket	*current;
	t_bucket	*next;
	ssize_t		i;

	if (!hashmap)
		return ;
	i = 0;
	while (i < hashmap->size)
	{
		current = hashmap->buckets[i];
		while (current)
		{
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
		i++;
	}
	free(hashmap->buckets);
	free(hashmap);
}

void	free_sig(t_sigenv *sigenv)
{
	if (!sigenv)
		return ;
	if (sigenv->def)
	{
		free(sigenv->def);
		sigenv->def = NULL;
	}
	sigenv->env = NULL;
	free(sigenv);
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	if (env->vars)
		free_hashmap(env->vars);
	if (env->tokenizer)
	{
		free_tokens(env->tokenizer);
		free(env->tokenizer);
	}
	if (env->sigenv)
	{
		if (env->sigenv->env && env->sigenv->env == env)
			env->sigenv->env = NULL;
		free_sig(env->sigenv);
	}
	if (env->input)
	{
		free(env->input);
		env->input = NULL;
	}
	free(env);
}
