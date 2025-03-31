/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 21:35:07 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 01:45:45 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/env.h"
#include "../inc/tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*tmp;

	current = *tokens;
	// printf("Freeing tokens from: %p\n", (void*)tokenizer->tokens);
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp->value);
		free(tmp);
	}
	*tokens = NULL;
}

void	free_ast(t_ast_node *node)
{
	int	i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->arg_types)
		free(node->arg_types);
	free(node);
}

static void	free_hashmap(t_hashmap *hashmap)
{
	t_bucket	*current;
	t_bucket	*next;
	int			count;

	count = 0;
	if (!hashmap)
		return ;
	for (ssize_t i = 0; i < hashmap->size; i++)
	{
		current = hashmap->buckets[i];
		while (current)
		{
			next = current->next;
			count++;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
	}
	printf("Freed %d buckets\n", count);
	free(hashmap->buckets);
	free(hashmap);
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	free_hashmap(env->vars);
	free_tokens(&env->tokenizer->tokens);
	free(env->tokenizer);
	free(env->input);
	free(env);
}
