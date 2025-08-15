/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:38:12 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:57:18 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pipeline.h"

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

int	execute_group(t_env *env, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals(env, CHILD_SIG_CUSTOM);
		status = execute(env, node->group, EXIT);
		free_ast(&env->root);
		free_env(env);
		exit(status);
	}
	wait(&status);
	status = check_process_child_exit(env, status);
	return (status);
}

pid_t	execute_one_pipeline_cmd(t_env *env, t_list *pipeline,
		int prev_read_end, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	status = EXIT_FAILURE;
	if (!pipeline || !pipeline->content)
		return (-1);
	pid = fork();
	if (pid != 0)
	{
		return (pid);
	}
	setup_child_signals(env, CHILD_SIG_CUSTOM);
	if (pipe_fd[READ_END] != -1)
	{
		close(pipe_fd[READ_END]);
	}
	setup_child_fds(prev_read_end, pipe_fd[WRITE_END]);
	status = execute(env, (t_ast_node *)pipeline->content, EXIT);
	comprehensive_cleanup(&env);
	exit(status);
}

int	execute_pipeline(t_env *env, t_ast_node *node)
{
	t_list	*pipeline;
	int		status;

	pipeline = build_cmd_list(env, node);
	if (!pipeline)
		return (1);
	status = execute_pipeline_cmds(env, pipeline);
	if (env->pipeline != NULL)
		free_pipeline_list(&env->pipeline);
	return (status);
}
