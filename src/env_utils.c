/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:23:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/20 03:13:49 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/env.h"
#include "../inc/minishell.h"
#include <complex.h>

void expand_redirection(t_arena *arena, t_ast_node *node, t_env_var *env_list);

volatile sig_atomic_t	g_signal_received = 0;

void set_signal(int signum) {
	g_signal_received = signum;
}

int get_env_var_count(t_env_var *env_list) {
	int			count;
	t_env_var	*current;

	count = 0;
	current = env_list;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

char **get_env_array(t_arena *arena, t_env_var *env_list) {
	int			count;
	char		**env_array;
	t_env_var	*current;
	int			i;
	size_t		len;

	count = get_env_var_count(env_list);
	env_array = arena_malloc(arena, sizeof(char *) * (count + 1));
	if (!env_array)
		return NULL;
	current = env_list;
	i = 0;
	while (current && env_array) {
		len = strlen(current->key) + strlen(current->value) + 2;
		env_array[i] = arena_malloc(arena, len);
		if (env_array[i]) {
			snprintf(env_array[i], len, "%s=%s", current->key, current->value);
			i++;
		}
		current = current->next;
	}
	if (env_array)
        env_array[i] = NULL;
    return env_array;
}

char *get_env_value(t_env_var *env_list, const char *key) {
    t_env_var	*current;

	current = env_list;
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

char *extract_var_name(t_arena *arena, char **str) {
    const char *start = *str;
    size_t len = 0;

    while (**str && (isalnum(**str) || **str == '_')) {
        (*str)++;
        len++;
    }

    char *name = arena_malloc(arena, len + 1);
    if (name) {
        strncpy(name, start, len);
        name[len] = '\0';
    }
    return name;
}

void process_var(t_arena *arena, char **src, char **dst, t_env_var *env_list) {
	char	*name;
	char	*value;

	(*src)++;
	name = extract_var_name(arena, src);
	if (name)
		value = get_env_value(env_list, name);
	else
		value = NULL;
	if (value) {
		strcpy(*dst, value);
		*dst += strlen(value);
	}
}

size_t calc_expanded_size(char *str, t_env_var *env_list) {
	size_t		size;
	char		*p = str;
	char		*name;
	char		*value;

	size = 0;
	while (*p) {
		if (*p == '$' && *(p + 1) && (isalnum(*(p + 1)) || *(p + 1) == '_')) {
			p++;
			name = extract_var_name(NULL, &p);
			if (name)
				value = get_env_value(env_list, name);
			else
				value = NULL;
			if (value)
				size += strlen(value);
		} else {
			size++;
			p++;
		}
	}
	return size;
}

char *expand_variables(t_arena *arena, char *str, t_env_var *env_list) {
    char	*dst;
    char	*src;
	size_t	size;
	char	*expanded;
	
	if (!str)
		return NULL;
	size = calc_expanded_size(str, env_list);
	expanded = arena_malloc(arena, size + 1);
	dst = expanded;
	src = str;
	while (src && *src && expanded) {
		if (*src == '$' && *(src + 1) && (isalnum(*(src + 1)) || *(src + 1) == '_'))
			process_var(arena, &src, &dst, env_list);
		else
			*dst++ = *src++;
	}
	if (expanded)
		*dst = '\0';
	return expanded;
}

int match_wildcard(const char *pattern, const char *string) {
	if (!*pattern && !*string)
		return 1;
	if (*pattern == '*' && !*(pattern + 1))
		return 1;
	if (!*pattern || !*string)
		return 0;
	if (*pattern == '*')
		return match_wildcard(pattern + 1, string) ||
				match_wildcard(pattern, string + 1);
	if (*pattern == '?' || *pattern == *string)
		return match_wildcard(pattern + 1, string + 1);
	return 0;
}

char **process_wildcard(t_arena *arena, char *arg, int *count) {
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				match_count;

	dir = opendir(".");
	//TODO: potential magic number issue
	matches = arena_malloc(arena, sizeof(char *) * 100);
	match_count = 0;
	if(!dir || !matches)
		return NULL;
	while ((entry = readdir(dir)) != NULL && match_count < 99) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		if (match_wildcard(arg, entry->d_name)) {
			matches[match_count] = arena_malloc(arena, strlen(entry->d_name) + 1);
			if (matches[match_count]) {
				strcpy(matches[match_count], entry->d_name);
				match_count++;
			}
		}
	}
	closedir(dir);
	matches[match_count] = NULL;
	*count = match_count;
	return matches;
}

char **get_matches(t_arena *arena, char *arg, int *match_count) {
	if (strchr(arg, '*') || strchr(arg, '?'))
		return process_wildcard(arena, arg, match_count);
	*match_count = 0;
	return NULL;
}

char **expand_wildcards(t_arena *arena, char **args) {
	int		arg_count;
	int		exp_count;
	char	**expanded;
	int		match_count;
	char	**matches;

	arg_count = 0;
	exp_count = 0;
	expanded = arena_malloc(arena, sizeof(char *) * 1000);
	while (args[arg_count]) {
		match_count = 0;
		matches = get_matches(arena, args[arg_count], &match_count);
		while (match_count > 0 && exp_count <999) {
			expanded[exp_count++] = *matches++;
			match_count--;
		}
		if (match_count == 0)
			expanded[exp_count++] = args[arg_count];
		arg_count++;
	}
	expanded[exp_count] = NULL;
	return expanded;
}

void expand_command(t_arena *arena, t_ast_node *node, t_env_var *env_list) {
	int	i;

	i = 0;
	if (!node->args)
		return;
	while(node->args[i]) {
		node->args[i] = expand_variables(arena, node->args[i], env_list);
		i++;
	}
	node->args = expand_wildcards(arena, node->args);
}

void process_expansions(t_arena *arena, t_ast_node *node, t_env_var *env_list) {
    if (!node || g_signal_received) return;

    if (node->type == NODE_CMD)
        expand_command(arena, node, env_list);
    else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT || 
             node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
        expand_redirection(arena, node, env_list);
    else {
        if (node->left)
            process_expansions(arena, node->left, env_list);
        if (node->right)
            process_expansions(arena, node->right, env_list);
    }
}

void expand_redirection(t_arena *arena, t_ast_node *node, t_env_var *env_list) {
	if (node->args && node->args[0])
		node->args[0] = expand_variables(arena, node->args[0], env_list);
	if (node->right)
		process_expansions(arena, node->right, env_list);
}

void setup_signals(void) {
    signal(SIGINT, set_signal);
    signal(SIGQUIT, set_signal);
    signal(SIGTERM, set_signal);
}

void expand_ast(t_arena *arena, t_ast_node *root, t_env_var *env_list) {
    g_signal_received = 0;
    setup_signals();
    process_expansions(arena, root, env_list);
}

int execute_shell_command(t_arena *arena, t_ast_node *root, t_env_var *env_list) {
    int &env->last_exit_code = 0;

    g_signal_received = 0;
    setup_signals();
    expand_ast(arena, root, env_list);
    if (!g_signal_received)
        &env->last_exit_code = execute_node(arena, root, STDIN_FILENO, STDOUT_FILENO, 
                                 &&env->last_exit_code, env_list);
int			execute_node(t_ast_node *node, int in_fd, int out_fd, int *&env->last_exit_code);
    else if (g_signal_received == SIGINT)
        &env->last_exit_code = 130;
    return &env->last_exit_code;
}
