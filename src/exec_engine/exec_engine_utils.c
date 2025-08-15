/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:54:19 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 16:07:10 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_external_command(t_env *env, t_ast_node *node,
		t_execute_type exec_type)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (exec_type == EXIT)
		handle_execve(env, node);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			set_all_signals(NORMAL_MODE, env->sigenv);
			handle_execve(env, node);
		}
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		set_all_signals(MINI_MODE, env->sigenv);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (128 + WTERMSIG(status));
	}
	return (status);
}

int	execute_command(t_env *env, t_ast_node *node, t_execute_type exec_type)
{
	int	status;

	status = 0;
	if (cmd_is_builtin(env, node))
		status = execute_builtin_command(env, node);
	else
		status = execute_external_command(env, node, exec_type);
	return (status);
}

int	execute_logical_op(t_env *env, t_ast_node *node)
{
	int	left_status;
	int	right_status;

	left_status = execute(env, node->left, RETURN);
	env->last_exit_code = left_status;
	if (node->type == NODE_AND)
	{
		if (left_status == 0)
		{
			right_status = execute(env, node->right, RETURN);
			return (right_status);
		}
		return (left_status);
	}
	else if (node->type == NODE_OR)
	{
		if (left_status != 0)
		{
			right_status = execute(env, node->right, RETURN);
			return (right_status);
		}
		return (left_status);
	}
	return (1);
}

int	execute_node_by_type(t_env *env, t_ast_node *node, t_execute_type exec_type)
{
	int	status;

	status = 0;
	if (node->type == NODE_CMD)
	{
		execute_command_expansion(env, node);
		status = execute_command(env, node, exec_type);
	}
	else if (node->type == NODE_PIPE)
		status = execute_pipeline(env, node);
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
	{
		status = execute_redirections(env, node);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
		status = execute_logical_op(env, node);
	else if (node->type == NODE_GROUP)
		status = execute_group(env, node);
	else
	{
		perror("Unknown node type in execution\n");
		return (1);
	}
	return (status);
}

int	execute_ast(t_env *env, t_ast_node *root, t_execute_type exec_type)
{
	return (execute(env, root, exec_type));
}
