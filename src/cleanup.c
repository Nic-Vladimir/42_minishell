/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 21:35:07 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/23 21:40:42 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tokenizer.h"
#include "../inc/ast.h"
#include "../inc/env.h"
#include <stdlib.h>

void free_tokens(t_tokenizer_data *tokenizer) {
    t_token *current = tokenizer->tokens;
    while (current) {
        t_token *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    tokenizer->tokens = NULL;
    tokenizer->tail = NULL;
}

void free_ast(t_ast_node *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->args) {
        for (int i = 0; node->args[i]; i++) {
            free(node->args[i]);
        }
        free(node->args);
    }
    free(node);
}

static void free_hashmap(t_hashmap *hashmap) {
    if (!hashmap) return;
    for (ssize_t i = 0; i < hashmap->size; i++) {
        t_bucket *current = hashmap->buckets[i];
        while (current) {
            t_bucket *next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(hashmap->buckets);
    free(hashmap);
}

void free_env(t_env *env) {
    if (!env) return;
    free_hashmap(env->vars);
    free_tokens(env->tokenizer);
    free(env->tokenizer);
    free(env);
}

