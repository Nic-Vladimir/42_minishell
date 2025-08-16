/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:07:38 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:37:19 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pipeline.h"

void	free_pipeline_list(t_list **pipeline)
{
	t_list	*tmp;

	while (*pipeline)
	{
		tmp = (*pipeline)->next;
		free(*pipeline);
		*pipeline = tmp;
	}
	*pipeline = NULL;
}

static int	setup_pipe(t_list *pipeline, int *pipe_fd)
{
	if (pipeline->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	else
	{
		pipe_fd[READ_END] = -1;
		pipe_fd[WRITE_END] = -1;
	}
	return (0);
}

static int	handle_pipeline_error(t_list *pipeline, int *pipe_fd)
{
	if (pipeline->next)
	{
		close(pipe_fd[READ_END]);
		close(pipe_fd[WRITE_END]);
	}
	return (1);
}

static int	process_pipeline_cmd(t_env *env, t_list *pipeline,
		int *prev_read_end, pid_t *last_pid)
{
	int	pipe_fd[2];

	if (setup_pipe(pipeline, pipe_fd))
		return (1);
	*last_pid = execute_one_pipeline_cmd(env, pipeline, *prev_read_end,
			pipe_fd);
	if (*last_pid == -1)
		return (handle_pipeline_error(pipeline, pipe_fd));
	if (pipeline->next)
		prep_for_next_cmd(env, prev_read_end, pipe_fd);
	else if (*prev_read_end != -1)
		close(*prev_read_end);
	return (0);
}

int	execute_pipeline_cmds(t_env *env, t_list *pipeline)
{
	int		prev_read_end;
	pid_t	last_pid;
	t_list	*current;
	int		last_cmd_status;
	int		pipeline_count;

	prev_read_end = -1;
	last_pid = 0;
	last_cmd_status = 0;
	if (!pipeline)
		return (1);
	pipeline_count = ft_lstsize(pipeline);
	env->pipeline = pipeline;
	current = pipeline;
	while (current)
	{
		if (process_pipeline_cmd(env, current, &prev_read_end, &last_pid))
			return (1);
		current = current->next;
	}
	last_cmd_status = wait_for_children(env, last_pid, pipeline_count);
	ft_printf("\n");
	return (last_cmd_status);
}
