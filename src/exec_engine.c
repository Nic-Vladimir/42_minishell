/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matus <matus@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 16:54:14 by matus            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <unistd.h>

int	execute_builtin_command(t_env *env, t_ast_node *node, int in_fd, int out_fd);

void exec_error(const char *msg) {
    perror(msg);
}

int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_pipeline(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_redirections(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_logical_op(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_group(t_env *env, t_ast_node *node, int in_fd, int out_fd);

char	*get_expand_value(t_env *env, const char *input, int start, int end)
{
	char	*var_name;
	char	*value;
	int		len;

	len = end - start;
	if (len == 1 && input[start] == '?')
		return (ft_itoa(env->last_exit_code));
	if (len == 1 && input[start] == '$')
		return (ft_itoa(env->shell_pid));
	var_name = malloc(len + 1);
	if (!var_name)
		return NULL;
	ft_strlcpy(var_name, &input[start], len + 1);
	value = get_env_value(env, var_name);
	free(var_name);
	if (value)
		return ft_strdup(value);
	else
		return ft_strdup("");
}

char *expand_var(t_env *env, const char *input)
{
	char	*expanded;
	char	*temp;
	int		i;
	int		start;

	expanded = ft_strdup("");
	if (!expanded)
        return NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i+1])
		{
			i++;
			start = i;
			if (input[i] == '$' || input[i] == '?')
				i++;
			else
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
					i++;
			temp = get_expand_value(env, input, start, i);
			expanded = ft_strjoin_free(expanded, temp);
			free(temp);
		}
		else
		{
			temp = malloc(2);
			if (!temp)
			{
				free(expanded);
				return NULL;
			}
			temp[0] = input[i++];
			temp[1] = '\0';
			expanded = ft_strjoin_free(expanded, temp);
			free(temp);
		}
	}
	return (expanded);
}

void update_arg_types(t_ast_node *node, int star_index, int len_args, int len_expanded)
{
    int             i;
    int             j;
    t_token_type    *new_arg_types;
    int             new_size;

    // New size = args before * + expanded + args after *
    new_size = (star_index) + len_expanded + (len_args - star_index - 1);
    new_arg_types = malloc(sizeof(int) * new_size);
    if (!new_arg_types)
        return; // Handle allocation failure gracefully in real code

    // Copy types before the * argument
    i = 0;
    while (i < star_index)
    {
        new_arg_types[i] = node->arg_types[i];
        i++;
    }

    // Assign TOK_WORD to all expanded arguments
    j = 0;
    while (j < len_expanded)
    {
        new_arg_types[i] = TOK_WORD;
        i++;
        j++;
    }

    // Copy types after the * argument
    while (star_index + 1 < len_args)
    {
        new_arg_types[i] = node->arg_types[star_index + 1];
        i++;
        star_index++;
    }

    // Free old arg_types and replace
    free(node->arg_types);
    node->arg_types = new_arg_types;
}

char **copy_args(t_ast_node *node, int star_index, char **expanded)
{
    char    **args_copy;
    int     i;
    int     j;
    int     len_expanded;
    int     len_args;

    if (!node || !node->args || !expanded)
        return NULL;

    // Count total original args
    len_args = 0;
    while (node->args[len_args])
        len_args++;

    // Count expanded args
    len_expanded = 0;
    while (expanded[len_expanded])
        len_expanded++;

    // Allocate enough space: args before * + expanded + args after *
    args_copy = malloc(sizeof(char *) * (len_args + len_expanded));
    if (!args_copy)
        return NULL;

    // Copy args before the * (star_index)
    i = 0;
    while (i < star_index)
    {
        args_copy[i] = ft_strdup(node->args[i]);
        i++;
    }

    // Copy expanded args
    j = 0;
    while (j < len_expanded)
    {
        args_copy[i] = ft_strdup(expanded[j]);
        i++;
        j++;
    }

    // Copy remaining args after the *
    while (star_index + 1 < len_args)
    {
        args_copy[i] = ft_strdup(node->args[star_index + 1]);
        i++;
        star_index++;
    }

    args_copy[i] = NULL; // Null-terminate

    // Update node->arg_types
    update_arg_types(node, star_index, len_args, len_expanded);

    return args_copy;
}

void free_list(char **list)
{
    int i;

    i = 0;
    while (list[i])
    {
        free(list[i]);
        i++;
    }
    free(list);
}

int execute_command_filter(t_env *env, t_ast_node *node, int in_fd, int out_fd) 
{
    int     i;
	char	*temp;
	char	*expanded;
    char    *args_str;
    char    **args_list;
    char    **temp_args;

    /*
	//TODO: Debugging
    printf("Node type: %d\n", node->type);
    printf("Node args: %p\n", (void*)node->args);
    if (node->args) {
        for (int j = 0; node->args[j] != NULL; j++) {
            printf("Args Before execute_command [%d]: %s\n", j, node->args[j]);
        }
    }
    */

    i = 0;
	while (node->args[i] != NULL) {
		if (node->arg_types[i] == TOK_WORD && ft_strchr(node->args[i], '*'))
		{
			temp = node->args[i];
            args_str = expand_wildcard(temp);
            //printf("args_str: %s\n", args_str);
			if (!node->args[i])
				return 1;
            args_list = ft_split(args_str, ' ');
            free(args_str);
            temp_args = copy_args(node, i, args_list);
            free_list(node->args);
            node->args = temp_args;
            ft_free_split(args_list);
		}
		else if (node->arg_types[i] == TOK_WORD || node->arg_types[i] == TOK_DBQ_BLOCK)
		{
			temp = node->args[i];
			expanded = expand_var(env, temp);
			node->args[i] = ft_strdup(expanded);
			free(expanded);
			free(temp);
			if (!node->args[i])
				return 1;
		}
		i++;
	}
    return (execute_builtin_command(env, node, in_fd, out_fd)) ;
}

int	execute_builtin_command(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    if (node->args[0] == NULL)
        return 1;
    if (ft_strncmp(node->args[0], "cd", 3) == 0)
        return execute_cd(env, node, in_fd, out_fd);
    if (ft_strncmp(node->args[0], "echo", 5) == 0)
		return execute_echo(env, node, in_fd, out_fd);
    if (ft_strncmp(node->args[0], "pwd", 4) == 0)
		return execute_pwd(env, node, in_fd, out_fd);
    if (ft_strncmp(node->args[0], "export", 7) == 0)
		return execute_export(env, node, in_fd, out_fd);
    if (ft_strncmp(node->args[0], "env", 4) == 0)
		return execute_env(env, node, in_fd, out_fd);
    if (ft_strncmp(node->args[0], "unset", 6) == 0)
		return execute_unset(env, node);
    if (ft_strncmp(node->args[0], "exit", 6) == 0)
		return execute_exit(env);
    return (execute_command(env, node, in_fd, out_fd));
}

int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
    if (!node) return 0;
	//printf("Node type: %d, in_fd: %d\n", node->type, in_fd);
    switch (node->type) {
        case NODE_CMD:
            return execute_command_filter(env, node, in_fd, out_fd);
        case NODE_PIPE:
            return execute_pipeline(env, node, in_fd, out_fd);
        case NODE_REDIR_IN:
        case NODE_REDIR_OUT:
        case NODE_REDIR_APPEND:
        case NODE_HEREDOC:
            return execute_redirections(env, node, in_fd, out_fd);
        case NODE_AND:
        case NODE_OR:
            return execute_logical_op(env, node, in_fd, out_fd);
        case NODE_GROUP:
            return execute_group(env, node, in_fd, out_fd);
        default:
            fprintf(stderr, "Unknown node type in execution\n");
            return 1;
    }
}

int execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
	pid_t	pid;
	char	**envp;
	char	*exec_path;

	pid = fork();
	if (pid == 0) {
		if (in_fd != STDIN_FILENO) {
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO) {
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		envp = get_envp_from_hashmap(env);
		exec_path = find_executable(env, node->args[0]);
        if (exec_path == NULL)
        {
            perror("not valid exec_path");
            exit(127);
        }

        /*
		//TODO: Remove this debug print
		printf("\nArgs: [%s]", exec_path);
        for (int i = 0; node->args[i]; i++)
            printf(" [%s]", node->args[i]);
        printf("\n");
        */

		//printf("Child: in_fd: %d, exec_path: %s\n", in_fd, exec_path);
		execve(exec_path, node->args, envp);
		fprintf(stderr, "Command not found: %s\n", node->args[0]);
		exit(127);
	}
    int status;
    waitpid(pid, &status, 0);
    if (in_fd != STDIN_FILENO)
        close(in_fd);
    if (out_fd != STDOUT_FILENO)
        close(out_fd);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return 1;
}

int execute_pipeline(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
	int	pipefd[2];
	int	left_status;
	int	right_status;

	if (pipe(pipefd) < 0) {
		exec_error("pipe");
		return 1;
	}
	left_status = execute_node(env, node->left, in_fd, pipefd[1]);
	(void)left_status;
	close(pipefd[1]);
	right_status = execute_node(env, node->right, pipefd[0], out_fd);
	close(pipefd[0]);
	env->last_exit_code = right_status;
    return right_status;
}

int collect_heredoc(t_env *env, char *delimiter, int *write_fd)
{
    char    *line;
    char    *expanded;
    int     pipe_fds[2];
    int     status;

    if (pipe(pipe_fds) == -1)
    {
        perror("pipe failed");
        return (-1);
    }
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            if (line)
                free(line);
            break;
        }
        expanded = expand_var(env, line);
        if (expanded)
        {
            status = write(pipe_fds[1], expanded, ft_strlen(expanded));
            write(pipe_fds[1], "\n", 1);
            free(expanded);
        }
        free(line);
    }
    close(pipe_fds[1]);
    *write_fd = pipe_fds[0];
    return status;
}

int execute_redirections(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
    int new_fd;
    int heredoc_fd;
    int status;
    char *delimiter;

    new_fd = -1;
    heredoc_fd = -1;
    status = 0;
    delimiter = NULL;
    if (node->type == NODE_HEREDOC) {
        delimiter = node->args[0];
        if (collect_heredoc(env, delimiter, &heredoc_fd) == -1)
            return (1);
        new_fd = heredoc_fd;
		//printf("new_fd: %d, right: %p\n", new_fd, (void*)node->right);
    }
    if (node->type == NODE_REDIR_IN)
        new_fd = open(node->args[0], O_RDONLY);
    if (node->type == NODE_REDIR_OUT)
        new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (node->type == NODE_REDIR_APPEND)
        new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (new_fd == -1 && node->type != NODE_HEREDOC)
        return 1;
    if (node->type == NODE_REDIR_IN && in_fd != STDIN_FILENO)
        close(in_fd);
    if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND) && out_fd != STDOUT_FILENO)
        close(out_fd);
    if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
        status = execute_node(env, node->right, new_fd, out_fd);
    else
        status = execute_node(env, node->right, in_fd, new_fd);
    return status;
}

int execute_logical_op(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
	int	left_status;
	int right_status;

	left_status = execute_node(env, node->left, in_fd, out_fd);
	*&env->last_exit_code = left_status;
	if ((node->type == NODE_AND && left_status == 0) ||
	(node->type == NODE_OR && left_status != 0)) {
		right_status = execute_node(env, node->right, in_fd, out_fd);
		*&env->last_exit_code = right_status;
		return right_status;
	}
	return left_status;
}

int execute_group(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
	return execute_node(env, node->left, in_fd, out_fd);
}

int execute_ast(t_env *env, t_ast_node *root) {
    t_env       *env_cpy;
    t_ast_node  *root_cpy;

    env_cpy = env;
    root_cpy = root;
	return execute_node(env_cpy, root_cpy, STDIN_FILENO, STDOUT_FILENO);
}

