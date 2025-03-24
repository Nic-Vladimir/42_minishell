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
        t_token *next = current->next;  // Save next pointer before freeing
        free(current->value);           // Free the token’s string
        free(current);                  // Free the token struct
        current = next;
    }
    tokenizer->tokens = NULL;  // Reset head
    tokenizer->tail = NULL;    // Reset tail for safety
}

void free_ast(t_ast_node *node) {
    if (!node) return;          // Base case: empty node
    free_ast(node->left);       // Recursively free left subtree
    free_ast(node->right);      // Recursively free right subtree
    if (node->args) {           // Free the args array if it exists
        for (int i = 0; node->args[i]; i++) {
            free(node->args[i]);  // Free each argument string
        }
        free(node->args);         // Free the array itself
    }
    free(node);                 // Free the node struct
}

static void free_hashmap(t_hashmap *hashmap) {
    if (!hashmap) return;
    for (ssize_t i = 0; i < hashmap->size; i++) {
        t_bucket *current = hashmap->buckets[i];
        while (current) {
            t_bucket *next = current->next;  // Save next pointer
            free(current->key);             // Free key string
            free(current->value);           // Free value string
            free(current);                  // Free bucket struct
            current = next;
        }
    }
    free(hashmap->buckets);  // Free the buckets array
    free(hashmap);           // Free the hashmap struct
}

void free_env(t_env *env) {
    if (!env) return;
    free_hashmap(env->vars);      // Free the hashmap (see below)
    free_tokens(env->tokenizer);  // Free any remaining tokens
    free(env->tokenizer);         // Free the tokenizer struct
    free(env);                    // Free the env struct
}

