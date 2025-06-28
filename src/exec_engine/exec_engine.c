/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/22 20:23:15 by mgavorni         ###   ########.fr       */
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
		return (execute_exit(env, env->last_exit_code));
	return (execute_command(env, node, in_fd, out_fd));
}

static t_ast_node	**collect_pipeline_commands(t_ast_node *node, int *count)
{
	t_ast_node	**commands;
	int			i;
	t_ast_node	*current;

	commands = NULL;
	i = 0;
	// Count commands and allocate array
	*count = 0;
	current = node;
	while (current && current->type == NODE_PIPE)
	{
		(*count)++;
		current = current->left;
	}
	(*count)++; // Last command
	commands = malloc(sizeof(t_ast_node *) * (*count));
	if (!commands)
		return (NULL);
	// Collect commands (rightmost to leftmost)
	current = node;
	i = *count - 1;
	while (current && current->type == NODE_PIPE)
	{
		commands[i--] = current->right;
		current = current->left;
	}
	commands[i] = current; // Leftmost command
	return (commands);
}

static int	execute_child(t_env *env, t_ast_node *cmd, int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	// Execute the command (handles NODE_CMD, NODE_REDIR_*, etc.)
	exit(execute_node(env, cmd, STDIN_FILENO, STDOUT_FILENO));
}

int	execute_pipeline(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	int			count;
	t_ast_node	**commands;
	int			*pipefds;
	pid_t		*pids;
	int			child_in_fd;
	int			child_out_fd;
	int			status;

	commands = collect_pipeline_commands(node, &count);
	if (!commands)
	{
		perror("malloc");
		return (1);
	}
	pipefds = malloc(sizeof(int) * 2 * (count - 1));
	pids = malloc(sizeof(pid_t) * count);
	if (!pipefds || !pids)
	{
		perror("malloc");
		free(commands);
		free(pipefds);
		free(pids);
		return (1);
	}
	// Create pipes
	for (int i = 0; i < count - 1; i++)
	{
		if (pipe(pipefds + i * 2) < 0)
		{
			perror("pipe");
			free(commands);
			free(pipefds);
			free(pids);
			return (1);
		}
	}
	// Fork for each command
	for (int i = 0; i < count; i++)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			for (int j = 0; j < count - 1; j++)
			{
				close(pipefds[j * 2]);
				close(pipefds[j * 2 + 1]);
			}
			free(commands);
			free(pipefds);
			free(pids);
			return (1);
		}
		if (pids[i] == 0)
		{
			// Child process
			signal(SIGINT, SIG_DFL); // Restore default SIGINT behavior
			// Set input FD
			child_in_fd = (i == 0) ? in_fd : pipefds[(i - 1) * 2];
			// Set output FD
			child_out_fd = (i == count - 1) ? out_fd : pipefds[i * 2 + 1];
			// Close unused pipe FDs
			for (int j = 0; j < count - 1; j++)
			{
				if (j != i - 1)
					close(pipefds[j * 2]); // Close read ends
				if (j != i)
					close(pipefds[j * 2 + 1]); // Close write ends
			}
			execute_child(env, commands[i], child_in_fd, child_out_fd);
		}
	}
	// Parent: Close all pipe FDs
	for (int i = 0; i < count - 1; i++)
	{
		close(pipefds[i * 2]);
		close(pipefds[i * 2 + 1]);
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	// Wait for all children, capture last command's status
	status = 0;
	for (int i = 0; i < count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1) // Last command's status
		{
			if (WIFEXITED(status))
				env->last_exit_code = WEXITSTATUS(status);
			else
				env->last_exit_code = 128 + WTERMSIG(status);
		}
	}
	free(commands);
	free(pipefds);
	free(pids);
	return (env->last_exit_code);
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
		perror("Unknown node type in execution\n");
		return (1);
	}
}

int	execute_ast(t_env *env, t_ast_node *root)
{
	return (execute_node(env, root, STDIN_FILENO, STDOUT_FILENO));
}
