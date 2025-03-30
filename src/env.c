/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matus <matus@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:51:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 16:53:21 by matus            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_tokenizer_data *init_tok_data() {
	t_tokenizer_data	*tok_data;

	tok_data = (t_tokenizer_data *)malloc(sizeof(t_tokenizer_data));
	if (!tok_data)
		return NULL;
    tok_data->tokens = NULL;
    tok_data->tail = NULL;
    return tok_data;
}

static void insert_env_var(t_env *env, char *envp_entry) {
	char	**res;
    char	*key;
    char	*value;

	res = ft_split(envp_entry, '=');
    if (!res || !res[0] || !res[1]) {
		if (res)
			free(res);
		return;
	}
    key = malloc(strlen(res[0]) + 1);
    value = malloc(strlen(res[1]) + 1);
    strcpy(key, res[0]);
    strcpy(value, res[1]);

    hashmap_insert(env, key, value);
	ft_free_split(res);
}

t_env *init_env(char **envp) {
	t_env	*env;
	int		i;

	i = 0;
	env = (t_env *)malloc(sizeof(t_env));
    if (!env)
		return NULL;
	env->shell_pid = getpid();
	env->last_exit_code = 0;
	env->vars = (t_hashmap *)malloc(sizeof(t_hashmap));
	if (!env->vars)
		return NULL;
	env->vars->size = 100;
	env->vars->buckets = (t_bucket **)malloc(env->vars->size * sizeof(t_bucket *));
	if (env->vars->buckets == NULL)
		return NULL;
	while (i < env->vars->size)
		env->vars->buckets[i++] = NULL;
	i = 0;
	while (envp[i])
		insert_env_var(env, envp[i++]);
    env->tokenizer = init_tok_data();
    return env;
}

char *get_env_value(t_env *env, const char *key) {
	int			index;
	t_bucket	*current_node;

	index = djb2_hash(key) % env->vars->size;
	if (index < 0)
		index *= -1;
	current_node = env->vars->buckets[index];
	while (current_node) {
		if (strcmp(current_node->key, key) == 0) {
			return current_node->value;
		}
		current_node = current_node->next;
	}
	return NULL;
}

char *find_executable(t_env *env, const char *command) {
    if (ft_strchr(command, '/'))
        return strdup(command);
    // Get PATH from environment
    char *path_env = get_env_value(env, "PATH");
    if (!path_env)
        return NULL;
    char *path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    // Tokenize PATH by ':'
    char *dir;
    char *path_ptr = path_copy;
    char *saveptr;
    while ((dir = strtok_r(path_ptr, ":", &saveptr)) != NULL) {
        path_ptr = NULL; // For subsequent calls
        // Construct full path: dir + '/' + command
        size_t path_len = strlen(dir) + strlen(command) + 2; // +2 for '/' and null terminator
        char *full_path = malloc(path_len);
        if (!full_path)
            continue;
        snprintf(full_path, path_len, "%s/%s", dir, command);
        // Check if file exists and is executable
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        free(full_path);
    }
    free(path_copy);
    return NULL;
}

