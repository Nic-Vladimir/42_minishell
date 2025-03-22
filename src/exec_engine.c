/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/18 22:46:37 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
    if (!node)
        return 0;
    switch (node->type) {
        case NODE_CMD:
            return execute_command(env, node, in_fd, out_fd);
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
		}
		if (out_fd != STDOUT_FILENO) {
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
		}
		envp = get_envp_from_hashmap(env);
		exec_path = find_executable(env, node->args[0]);

		//TODO: Remove this debug print
		printf("\nArgs: [%s]", exec_path);
        for (int i = 0; node->args[i]; i++)
            printf(" [%s]", node->args[i]);
        printf("\n");

		execve(exec_path, node->args, envp);
		fprintf(stderr, "Command not found: %s\n", node->args[0]);
		exit(127);
	} else {
		wait(NULL);
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

int execute_redirections(t_env *env, t_ast_node *node, int in_fd, int out_fd) {
	int	new_fd;

	new_fd = -1;
	switch (node->type) {
		case NODE_REDIR_IN:
			new_fd = open(node->args[0], O_RDONLY);
			break;
		case NODE_REDIR_OUT:
			new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			break;
		case NODE_REDIR_APPEND:
			new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
			break;
		case NODE_HEREDOC:
			fprintf(stderr, "Heredoc not implemented yet\n");
            return 1;
		default:
		return 1;
	}
    if (node->type == NODE_REDIR_IN && in_fd != STDIN_FILENO)
        close(in_fd);
    else if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND) && out_fd != STDOUT_FILENO)
        close(out_fd);
    if (node->type == NODE_REDIR_IN)
        return execute_node(env, node->right, new_fd, out_fd);
    else
        return execute_node(env, node->right, in_fd, new_fd);
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

