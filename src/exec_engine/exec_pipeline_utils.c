/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:07:38 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/29 03:55:12 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include "../../inc/pipeline.h"

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

int	wait_for_children(t_env *env, pid_t last_pid, int pipeline_count)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;

	last_cmd_status = 0;
	while (pipeline_count--)
	{
		child_pid = wait(&status);
		if (child_pid == last_pid)
			last_cmd_status = check_process_child_exit(env, status);
		else
			check_process_child_exit(env, status);
	}
	return (last_cmd_status);
}
