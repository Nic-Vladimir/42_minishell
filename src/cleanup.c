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

#include "../inc/tokenizer.h"
#include "../inc/ast.h"
#include "../inc/env.h"
#include <stdio.h>
#include <stdlib.h>

void free_tokens(t_tokenizer_data *tokenizer)
{
    t_token *current;
    t_token *next;

    current = tokenizer->tokens;
    //printf("Freeing tokens from: %p\n", (void*)tokenizer->tokens);
    while (current) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    tokenizer->tokens = NULL;
    tokenizer->tail = NULL;
}

void free_ast(t_ast_node *node)
{
    int i;

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

static void free_hashmap(t_hashmap *hashmap)
{
    t_bucket    *current;
    t_bucket    *next;

    if (!hashmap) return;
    for (ssize_t i = 0; i < hashmap->size; i++) {
        current = hashmap->buckets[i];
        while (current) {
            next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(hashmap->buckets);
    free(hashmap);
}

void free_env(t_env *env)
{
    if (!env) return;
    free_hashmap(env->vars);
    free_tokens(env->tokenizer);
    free(env->tokenizer);
    free(env->input);
    free(env);
}

