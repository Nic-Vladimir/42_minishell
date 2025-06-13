/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:16:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/13 14:59:24 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

void	setup_child_fds(int in_fd, int out_fd)
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
}

 void	execute_child_process(t_env *env, t_ast_node *node)
{
	char	**envp;
	char	*exec_path;

	envp = get_envp_from_hashmap(env);
	exec_path = find_executable(env, node->args[0]);
	if (exec_path == NULL)
	{
		ft_printf("minishell: %s: Is not an executable binary\n",
			node->args[0]);
		free_ast(&env->root);
		free_env(env);
		free_envp(envp);
		exit(127);
	}
	if (execve(exec_path, node->args, envp) == -1)
	{
		ft_printf("minishell: %s: Is not an executable binary\n", exec_path);
		free_ast(&env->root);
		free(exec_path);
		free_envp(envp);
		free_env(env);
		exit(127);
	}
}

int	execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{	
		set_all_signals(NORMAL_MODE, env->sigenv);
		setup_child_fds(in_fd, out_fd);
		execute_child_process(env, node);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	set_all_signals(MINI_MODE, env->sigenv);
	write(STDOUT_FILENO, "\n", 1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}
