/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:38:12 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/29 02:54:41 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pipeline.h"

static void	wait_for_pipeline(t_env *env, t_pipeline *pl)
{
	int	status;
	int	i;

	i = 0;
	while (i < pl->count)
	{
		waitpid(pl->pids[i], &status, 0);
		if (i == pl->count - 1)
		{
			if (WIFEXITED(status))
				env->last_exit_code = WEXITSTATUS(status);
			else
				env->last_exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
}

int	execute_pipeline(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	t_pipeline	pl;
	int			i;

	i = 0;
	if (init_pipeline(node, &pl))
		return (1);
	if (setup_pipes(&pl))
		return (free_pipeline(&pl), 1);
	if (manage_children(env, &pl, in_fd, out_fd))
		return (free_pipeline(&pl), 1);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	while (i < pl.count)
	{
		wait_for_pipeline(env, &pl);
		i++;
	}
	return (free_pipeline(&pl), env->last_exit_code);
}
