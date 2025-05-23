/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/23 16:05:26 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/minishell.h"
#include <unistd.h>

void exec_error(const char *msg) {
    perror(msg);
}

int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_pipeline(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_redirections(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_logical_op(t_env *env, t_ast_node *node, int in_fd, int out_fd);
int execute_group(t_env *env, t_ast_node *node, int in_fd, int out_fd);

//TODO: This is ass, doesn't work, rewrite and fix
char *expand_vars_DBQ(t_env *env, const char *input) {
	int		i;
	char	*expanded;
	char	var_name[256];
	int		var_len;
	char	*temp;

	i = 0;
	var_len = 0;
	expanded = ft_strdup("");
	while (input[i]) {
		if (input[i] == '$') {
			i++;
			if (input[i] == '$') {
				temp = ft_itoa(env->shell_pid);
				i++;
			} else if (input[i] == '?') {
				temp = ft_itoa(env->last_exit_code);
				i++;
			} else {
				var_len = 0;
				while (ft_isalnum(input[i]) || input[i] == '_') {
					var_name[var_len++] = input[i++];
				}
				var_name[var_len] = '\0';
				temp = get_env_value(env, var_name);
				if (!temp)
					temp = ft_strdup("");
				else
					temp = ft_strdup(temp);
			}
			expanded = ft_strjoin_free(expanded, temp);
			free(temp);
		} else {
			char str[2] = {input[i], '\0'};
			expanded = ft_strjoin_free(expanded, str);
			i++;
		}
	}
	return expanded;
}


int execute_command_filter(t_env *env, t_ast_node *node, int in_fd, int out_fd) 
{
    int     i;
    char    *env_value;
    size_t  env_value_len;

	/*
    printf("Node type: %d\n", node->type);
    printf("Node args: %p\n", (void*)node->args);
    if (node->args) {
        for (int j = 0; node->args[j] != NULL; j++) {
            printf("Args[%d]: %s\n", j, node->args[j]);
        }
    }
	*/
    i = 1;
	while (node->args[i] != NULL) {
		if (node->type == NODE_DBQ) {
			char *expanded = expand_vars_DBQ(env, node->args[i]);
			node->args[i] = (char *)malloc(ft_strlen(expanded) + 1);
			ft_strlcpy(node->args[i], expanded, ft_strlen(expanded) + 1);
			free(expanded);
		}
		i++;
	}
	if (node->type == NODE_CMD) {
		while (node->args[i] != NULL)
		{
			if (node->args[i][0] == '$')
			{
				if (node->args[i][1] == '?' && node->args[i][2] == '\0')
				{
					env_value = ft_itoa(env->last_exit_code);
					env_value_len = ft_strlen(env_value) + 1;
					node->args[i] = (char *)malloc(env_value_len);
					ft_strlcpy(node->args[i], env_value, env_value_len);
					i++;
					continue;
				}
				else if (node->args[i][1] == '$' && node->args[i][2] == '\0')
				{
					env_value = ft_itoa(env->shell_pid);
					env_value_len = ft_strlen(env_value) + 1;
					node->args[i] = (char *)malloc(env_value_len);
					ft_strlcpy(node->args[i], env_value, env_value_len);
					i++;
					continue;
				}
				env_value = get_env_value(env, &node->args[i][1]);
				env_value_len = ft_strlen(env_value) + 1;
				node->args[i] = (char *)malloc(env_value_len);
				ft_strlcpy(node->args[i], env_value, env_value_len);
			}
			i++;
		}
	}
    return (execute_command(env, node, in_fd, out_fd));
}

int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
    if (!node) return 0;
	printf("Node type: %d, in_fd: %d\n", node->type, in_fd);
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

char *collect_heredoc(char *delimiter, int *write_fd) {
    char *line;
    char *result;
    int pipe_fds[2];
    int status;

    line = NULL;
    result = NULL;
    pipe_fds[0] = 0;
    pipe_fds[1] = 0;
    status = pipe(pipe_fds);
	set_all_signals(NORMAL_MODE, &g_glob_sig.def);
    if (status == -1) {
        perror("pipe failed");
        return NULL;
    }
    while (1) {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0) {
            if (line) free(line);
            break;
        }
        status = write(pipe_fds[1], line, strlen(line));
        write(pipe_fds[1], "\n", 1);
        free(line);
    }
    close(pipe_fds[1]);
    *write_fd = pipe_fds[0];
	//printf("pipe_fds[0]: %d\n", pipe_fds[0]);
    return result;
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
        collect_heredoc(delimiter, &heredoc_fd);
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

	return execute_node(env, root, STDIN_FILENO, STDOUT_FILENO);
}

