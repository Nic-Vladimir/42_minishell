/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:16:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/24 11:16:33 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	setup_child_fds(t_ast_node *node, int in_fd, int out_fd)
{
	if (node->args && node->args[0] && strcmp(node->args[0], "cat") == 0
		&& node->args[1] == NULL)
		in_fd = STDIN_FILENO;
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
}

static int	execute_external_command(t_env *env, t_ast_node *node)
{
	char	**envp;
	char	*exec_path;

	envp = get_envp_from_hashmap(env);
	exec_path = find_executable(env, node->args[0]);
	if (exec_path == NULL)
	{
		perror("Not valid exec_path");
		free_ast(node);
		free_env(env);
		free_envp(envp);
		exit(127);
	}
	if (execve(exec_path, node->args, envp) == -1)
	{
		free_ast(node);
	}
	fprintf(stderr, "Command not found: %s\n", node->args[0]);
	free_envp(envp);
	exit(127);
}

static int	execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd)
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
	return (execute_external_command(env, node));
}

int	create_child_process(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	pid_t	pid;
	int		status;

	if (ft_strncmp(node->args[0], "exit", 6) == 0)
		return (execute_exit(env));
	pid = fork();
	if (pid == 0)
	{
		setup_child_fds(node, in_fd, out_fd);
		status = execute_command(env, node, STDIN_FILENO, STDOUT_FILENO);
		free_ast(env->root);
		free_env(env);
		exit(status);
	}
	waitpid(pid, &status, 0);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}
