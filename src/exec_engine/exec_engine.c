/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/03 18:59:15 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin_command(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	if (node->args[0] == NULL)
		return (1);
	if (ft_strncmp(node->args[0], "cd", 3) == 0)
		return (execute_cd(env, node, in_fd, out_fd));
	if (ft_strncmp(node->args[0], "echo", 5) == 0)
		return (execute_echo(env, node, in_fd, out_fd));
	if (ft_strncmp(node->args[0], "pwd", 4) == 0)
		return (execute_pwd(env, node, in_fd, out_fd));
	if (ft_strncmp(node->args[0], "export", 7) == 0)
		return (execute_export(env, node, in_fd, out_fd));
	if (ft_strncmp(node->args[0], "env", 4) == 0)
		return (execute_env(env, node, in_fd, out_fd));
	if (ft_strncmp(node->args[0], "unset", 6) == 0)
		return (execute_unset(env, node));
	if (ft_strncmp(node->args[0], "exit", 6) == 0)
		return (execute_exit(env));
	return (execute_command(env, node, in_fd, out_fd));
}

static int	execute_pipeline(t_env *env, t_ast_node *node, int in_fd,
		int out_fd)
{
	int	pipefd[2];
	int	left_status;
	int	right_status;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	left_status = execute_node(env, node->left, in_fd, pipefd[1]);
	(void)left_status;
	close(pipefd[1]);
	right_status = execute_node(env, node->right, pipefd[0], out_fd);
	close(pipefd[0]);
	env->last_exit_code = right_status;
	return (right_status);
}

static int	execute_logical_op(t_env *env, t_ast_node *node, int in_fd,
		int out_fd)
{
	int	left_status;
	int	right_status;

	left_status = execute_node(env, node->left, in_fd, out_fd);
	*&env->last_exit_code = left_status;
	if ((node->type == NODE_AND && left_status == 0) || (node->type == NODE_OR
			&& left_status != 0))
	{
		right_status = execute_node(env, node->right, in_fd, out_fd);
		*&env->last_exit_code = right_status;
		return (right_status);
	}
	return (left_status);
}

int	execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	if (!node)
		return (0);
	if (node->type == NODE_CMD)
		return (execute_command_expansion(env, node, in_fd, out_fd));
	else if (node->type == NODE_PIPE)
		return (execute_pipeline(env, node, in_fd, out_fd));
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
		return (execute_redirections(env, node, in_fd, out_fd));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (execute_logical_op(env, node, in_fd, out_fd));
	else if (node->type == NODE_GROUP)
		return (execute_node(env, node->left, in_fd, out_fd));
	else
	{
		fprintf(stderr, "Unknown node type in execution\n");
		return (1);
	}
}

int	execute_ast(t_env *env, t_ast_node *root)
{
	return (execute_node(env, root, STDIN_FILENO, STDOUT_FILENO));
}
