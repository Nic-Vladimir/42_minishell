/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:48:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/24 17:17:55 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef MINISHELL_H
# define MINISHELL_H

// # include "../lib/libft/inc/libft.h"
// # include "ast.h"
// # include "colors.h"
// # include "env.h"
// # include "sig_hand.h"
// # include "tokenizer.h"
// # include <dirent.h>
// # include <fcntl.h>
// # include <limits.h>
// # include <readline/history.h>
// # include <readline/readline.h>
// # include <stdbool.h>
// # include <stdio.h>
// # include <stdlib.h>
// # include <string.h>
// # include <sys/wait.h>
// # include <unistd.h>

# include "../lib/libft/inc/libft.h"
# include "colors.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env			t_env;
typedef struct s_ast_node		t_ast_node;
typedef struct s_token			t_token;
typedef struct s_tokenizer_data	t_tokenizer_data;

# include "ast.h"
# include "env.h"
# include "sig_hand.h"
# include "tokenizer.h"

# define PROMPT_PREFIX_OK "\001\033[38;5;238m\002╭─\001\033[38;5;255;48;5;238m\002 ₘᵢₙᵢ🐚 \001\033[38;5;238;48;5;42m\002 ✔ \001\033[38;5;42;48;5;238m\002\001\033[38;5;255m\002  "
# define PROMPT_PREFIX_KO "\001\033[38;5;238m\002╭─\001\033[38;5;255;48;5;238m\002 ₘᵢₙᵢ🐚 \001\033[38;5;238;48;5;1m\002 ✘ \001\033[38;5;1;48;5;238m\002\001\033[38;5;255m\002  "
# define PROMPT_SUFFIX "\001\033[0m\002\001\033[38;5;238m\002\n╰─\001\033[0m\002 "

t_token							*tokenize(t_tokenizer_data *data, char *input);
t_ast_node						*parse(t_tokenizer_data *data);
void							debug_ast(t_ast_node *root);
int								execute_node(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								hashmap_insert(t_env *env, char *key,
									char *value);
t_env							*init_env(char **envp);
char							**get_envp_from_hashmap(t_env *env);
int								execute_ast(t_env *env, t_ast_node *root);
int								djb2_hash(const char *key);
char							*find_executable(t_env *env,
									const char *command);
char							*get_env_value(t_env *env, const char *key);
char							*get_prompt(t_env *env);
void							print_transient_prompt(char *command);
void							free_tokens(t_tokenizer_data *tokenizer);
void							free_ast(t_ast_node *node);
void							free_env(t_env *env);
char							*expand_wildcard(char *arg);
int								execute_export(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_env(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_pwd(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_echo(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_cd(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_exit(t_env *env);
int								execute_unset(t_env *env, t_ast_node *node);

void							clean_rl(void);
void							ft_free_split(char **res);
void							free_envp(char **envp);
// void			init_t_ast(t_ast_node *node);

char							*process_pipe(t_tokenizer_data *tok_data,
									char *input);
char							*process_ampersand(t_tokenizer_data *tok_data,
									char *input);
char							*process_redir_in(t_tokenizer_data *tok_data,
									char *input);
char							*process_redir_out(t_tokenizer_data *tok_data,
									char *input);
char							*process_simple_token(t_tokenizer_data *tok_data,
									char *input);
void							add_token(t_tokenizer_data *tok_data,
									t_token_type type, char *value, size_t len);
char							*process_single_quotes(t_tokenizer_data *tok_data,
									char *input);
char							*process_double_quotes(t_tokenizer_data *tok_data,
									char *input);
char							*process_word_token(t_tokenizer_data *tok_data,
									char *input);
void							sort_envp(char **envp, int low, int high);
void							format_envp(char **envp);
char							*handle_quotes(t_ast_node *node, int *i);
int								execute_redirections(t_env *env,
									t_ast_node *node, int in_fd, int out_fd);
int								collect_heredoc(t_env *env, char *delimiter,
									int *write_fd);
char							*expand_var(t_env *env, const char *input);
char							**copy_args(t_ast_node *node, int star_index,
									char **expanded);
int								execute_command(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								execute_command_expansion(t_env *env,
									t_ast_node *node, int in_fd, int out_fd);
int								execute_builtin_command(t_env *env,
									t_ast_node *node, int in_fd, int out_fd);
t_ast_node						*ast_new_node(t_node_type type, char **args);
t_ast_node						*parse_redirection(t_tokenizer_data *tok_data,
									t_ast_node *cmd);
t_ast_node						*parse_simple_command(t_tokenizer_data *tok_data);
t_ast_node						*init_cmd_node(char **args, int arg_count);
t_ast_node						*ast_node_insert(t_ast_node *root,
									t_node_type type, char **args);
bool							is_operator_token(t_token_type type);
bool							is_redirection_token(t_token_type type);
bool							validate_special_cases(t_token *current);
bool							validate_single_token(t_token *current,
									t_token *prev, int *paren_count);
void							update_arg_types(t_ast_node *node,
									int star_index, int len_args,
									int len_expanded);
int								calculate_total_args(t_ast_node *node,
									char **expanded, int *len_args,
									int *len_expanded);

#endif
