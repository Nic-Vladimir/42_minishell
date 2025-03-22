/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:51:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/18 22:39:23 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_tokenizer_data *init_tok_data(t_arena *arena) {
	t_tokenizer_data	*tok_data;

	tok_data = (t_tokenizer_data *)arena_malloc(arena, sizeof(t_tokenizer_data));
	if (!tok_data)
		return NULL;
    tok_data->tokens = NULL;
    tok_data->tail = NULL;
	tok_data->arena = arena;
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
    key = arena_malloc(env->arena, strlen(res[0]) + 1);
    value = arena_malloc(env->arena, strlen(res[1]) + 1);
    strcpy(key, res[0]);
    strcpy(value, res[1]);

    hashmap_insert(env, key, value);
	free(res);
}

t_env *init_env(t_arena *arena, char **envp) {
	t_env	*env;
	int		i;

	i = 0;
	env = (t_env *)arena_malloc(arena, sizeof(t_env));
    if (!env)
		return NULL;
	env->shell_pid = getpid();
	env->arena = arena;
	env->last_exit_code = 0;
	env->vars = (t_hashmap *)arena_malloc(arena, sizeof(t_hashmap));
	if (!env->vars)
		return NULL;
	env->vars->size = 100;
	env->vars->buckets = (t_bucket **)arena_malloc(arena, env->vars->size * sizeof(t_bucket *));
	if (env->vars->buckets == NULL)
		return NULL;
	while (i < env->vars->size)
		env->vars->buckets[i++] = NULL;
	i = 0;
	while (envp[i])
		insert_env_var(env, envp[i++]);
    env->tokenizer = init_tok_data(arena);
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
/*
char *find_executable(t_env *env, const char *command) {
    // If command contains '/', treat it as a full path
    if (strchr(command, '/')) {
        char *result = strdup(command);
        if (!result) {
            perror("strdup failed");
            return NULL;
        }
        if (access(result, X_OK) == 0) {
            return result;
        }
        free(result);
        return NULL;
    }

    // Construct path using /bin/ as the standard location
    const char *standard_dir = "/bin";
    size_t path_len = strlen(standard_dir) + strlen(command) + 2; // +2 for '/' and '\0'
    char *full_path = (char *)arena_malloc(env->arena, path_len);
    if (!full_path) {
        perror("malloc failed");
        return NULL;
    }

    snprintf(full_path, path_len, "%s/%s", standard_dir, command);

    // Check if file exists and is executable in /bin/
    if (access(full_path, X_OK) == 0) {
        return full_path;
    }

    return NULL;
}
*/
