/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:48:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/18 11:17:23 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/inc/libft.h"
# include "ast.h"
# include "colors.h"
# include "env.h"
# include "pedo.h"
# include "sig_hand.h"
# include "tokenizer.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env			t_env;
typedef struct s_ast_node		t_ast_node;
typedef struct s_token			t_token;
typedef struct s_tokenizer_data	t_tokenizer_data;
typedef struct s_child_data		t_child_data;
typedef struct s_heredoc_data	t_heredoc_data;

# define READ_END 0
# define WRITE_END 1

# define PROMPT_PREFIX_OK                                \
	"\001\033[38;5;238m\002╭─"                   \
	"\001\033[38;5;255;48;5;238m\002 ₘᵢₙᵢ🐚 " \
	"\001\033[38;5;238;48;5;42m\002 ✔ "            \
	"\001\033[38;5;42;48;5;238m\002"                 \
	"\001\033[38;5;255m\002  "

# define PROMPT_PREFIX_KO                                \
	"\001\033[38;5;238m\002╭─"                   \
	"\001\033[38;5;255;48;5;238m\002 ₘᵢₙᵢ🐚 " \
	"\001\033[38;5;238;48;5;1m\002 ✘ "             \
	"\001\033[38;5;1;48;5;238m\002"                  \
	"\001\033[38;5;255m\002  "

# define PROMPT_SUFFIX                   \
	"\001\033[0m\002"                   \
	"\001\033[38;5;238m\002\n╰─" \
	"\001\033[0m\002 "

t_token							*tokenize(t_tokenizer_data *data, char *input);
t_ast_node						*parse(t_tokenizer_data *data);
void							debug_ast(t_ast_node *root);
int								execute_node(t_env *env, t_ast_node *node,
									int in_fd, int out_fd);
int								hashmap_insert(t_env *env, char *key,
									char *value);
t_env							*init_env(char **envp);
char							**get_envp_from_hashmap(t_env *env);
int								execute_ast(t_env *env, t_ast_node *root,
									t_execute_type exec_type);
int								djb2_hash(const char *key);
char							*find_executable(t_env *env,
									const char *command);
char							*get_env_value(t_env *env, const char *key);
char							*get_prompt(t_env *env);
void							print_transient_prompt(char *command);
void							free_tokens(t_tokenizer_data *tokenizer);
void							free_ast(t_ast_node **node);
void							free_env(t_env *env);
char							*expand_wildcard(char *arg);
int								execute_export(t_env *env, t_ast_node *node);
int								execute_env(t_env *env, t_ast_node *node);
int								execute_pwd(t_env *env, t_ast_node *node);
int								execute_echo(t_env *env, t_ast_node *node);
int								execute_cd(t_env *env, t_ast_node *node);
int								execute_exit(t_env *env, t_ast_node *node,
									int sig);
int								execute_unset(t_env *env, t_ast_node *node);

void							clean_rl(void);
void							ft_free_split(char **res);
void							free_envp(char **envp);

bool							is_valid_token_char(char curent_char);

char							*process_invalid_character(
									t_tokenizer_data *tok_data,
									char *input);
char							*process_pipe(t_tokenizer_data *tok_data,
									char *input);
char							*process_ampersand(t_tokenizer_data *tok_data,
									char *input);
char							*process_redir_in(t_tokenizer_data *tok_data,
									char *input);
char							*process_redir_out(t_tokenizer_data *tok_data,
									char *input);
char							*process_simple_token(
									t_tokenizer_data *tok_data,
									char *input);
void							add_token(t_tokenizer_data *tok_data,
									t_token_type type, char *value, size_t len);
char							*process_single_quotes(
									t_tokenizer_data *tok_data,	char *input);
char							*process_double_quotes(
									t_tokenizer_data *tok_data,
									char *input);
char							*process_word_token(t_tokenizer_data *tok_data,
									char *input);
void							sort_envp(char **envp, int low, int high);
void							format_envp(char **envp);
char							*handle_quotes(t_ast_node *node, int *i);
int								execute_redirections(t_env *env,
									t_ast_node *node);
int								collect_heredoc(t_env *env, char *delimiter,
									int *write_fd);
char							*expand_var(t_env *env, const char *input);
char							**copy_args(t_ast_node *node, int star_index,
									char **expanded);
int								execute_command(t_env *env, t_ast_node *node,
									t_execute_type exec_type);

bool							cmd_is_builtin(t_env *env, t_ast_node *node);
void							handle_execve(t_env *env, t_ast_node *node);
int								execute_group(t_env *env, t_ast_node *node);
pid_t							execute_one_pipeline_cmd(t_env *env,
									t_list *pipeline, int prev_read_end,
									int *pipe_fd);
void							prep_for_next_cmd(t_env *env,
									int *prev_read_end, int *pipe_fd);
int								wait_for_children(t_env *env, pid_t last_pid,
									int pipeline_count);
int								check_process_child_exit(t_env *env,
									int status);
t_list							*build_cmd_list(t_env *env, t_ast_node *node);
int								execute_pipeline_cmds(t_env *env,
									t_list *pipeline);

void							execute_command_expansion(t_env *env,
									t_ast_node *node);
int								execute_builtin_command(t_env *env,
									t_ast_node *node);
int								execute_node_by_type(t_env *env,
									t_ast_node *node, t_execute_type exec_type);

t_ast_node						*ast_new_node(t_node_type type, char **args);
t_ast_node						*parse_redirection(t_tokenizer_data *tok_data,
									t_ast_node *cmd);
t_ast_node						*parse_simple_command(
									t_tokenizer_data *tok_data);
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

int								process_heredoc_input(t_env *env,
									char *delimiter, int write_fd);
int								execute_in_child(t_env *env,
									t_child_data *child_data, int in_fd,
									int out_fd);
int								collect_heredoc(t_env *env, char *delimiter,
									int *write_fd);
void							init_structs(t_heredoc_data *hd,
									t_child_data *child);
void							child_linker(t_child_data *child,
									t_heredoc_data *data,
									int (*func)(void *data));
void							herdoc_linker(t_heredoc_data *hd, t_env *env,
									char *delimiter);
void							setup_child_fds(int in_fd, int out_fd);
void							reset_terminal_for_readline(void);
void							execute_child_process(t_env *env,
									t_ast_node *node);
void							setup_child_signals(t_env *env, int behavior);
int								free_bucket(t_bucket *new_bucket);
int								execute(t_env *env, t_ast_node *node,
									t_execute_type exec_type);
int								execute_pipeline(t_env *env, t_ast_node *node);
void							free_pipeline_list(t_list **pipeline);
int								execute_cleaning(t_env *env);

void							free_everything(t_env **env);
void							force_cleanup_all_memory(t_env **env);
void							report_memory_usage(t_env *env);
void							child_comprehensive_cleanup(t_env *env);
void							comprehensive_cleanup(t_env **env);

void							do_ast(t_env **env);
void							do_tokens(t_env **env);
void							do_pipeline(t_env **env);
void							do_input(t_env **env);
void							free_hashmap(t_hashmap *hashmap);
void							free_sig(t_sigenv *sigenv);

pid_t							ft_getpid(void);
#endif
