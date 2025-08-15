/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:58:45 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 15:00:04 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_execve(t_env *env, t_ast_node *node)
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

int	check_process_child_exit(t_env *env, int status)
{
	int	signal;

	(void)env;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		return (128 + signal);
	}
	else
		return (EXIT_FAILURE);
}
