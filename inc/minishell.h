/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:48:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/23 22:35:38 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
#define MINISHELL_H

#include <dirent.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../lib/libft/inc/libft.h"
#include "colors.h"

typedef struct s_env t_env;
typedef struct s_ast_node t_ast_node;
typedef struct s_token t_token;
typedef struct s_tokenizer_data t_tokenizer_data;

#include "sig_hand.h"
#include "tokenizer.h"
#include "ast.h"
#include "env.h"

t_token *tokenize(t_tokenizer_data *data, char *input);
t_ast_node *parse(t_tokenizer_data *data);
void debug_ast(t_ast_node *root);
int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int hashmap_insert(t_env *env, char *key, char *value);
t_env *init_env(char **envp);
char **get_envp_from_hashmap(t_env *env);
int execute_ast(t_env *env, t_ast_node *root);
int djb2_hash(const char *key);
char *find_executable(t_env *env, const char *command);
char *get_env_value(t_env *env, const char *key);
char *get_prompt(t_env *env);
void print_transient_prompt(char *command);
void free_tokens(t_tokenizer_data *tokenizer);
void free_ast(t_ast_node *node);
void free_env(t_env *env);

#endif