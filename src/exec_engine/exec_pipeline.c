/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:38:12 by vnicoles          #+#    #+#             */
/*   Updated: 2025/07/28 13:41:02 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pipeline.h"

static void	prep_for_next_cmd(t_env *env, int *prev_read_end, int *pipe_fd)
{
	(void)env;
	if (*prev_read_end != -1)
		close(*prev_read_end);
	close(pipe_fd[WRITE_END]);
	*prev_read_end = pipe_fd[READ_END];
}

static pid_t	execute_one_pipeline_cmd(t_env *env, t_list *pipeline,
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
	exit(status);
}

int	execute_pipeline_cmds(t_env *env, t_list *pipeline)
{
	int		pipe_fd[2];
	int		prev_read_end;
	pid_t	last_pid;
	int		last_cmd_status;
	int		pipeline_count;

	if (!pipeline)
		return (1);
	pipeline_count = ft_lstsize(pipeline);
	last_pid = 0;
	prev_read_end = -1;
	env->pipeline = pipeline;
	while (pipeline)
	{
		if (pipeline->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		else
		{
			pipe_fd[READ_END] = -1;
			pipe_fd[WRITE_END] = -1;
		}
		last_pid = execute_one_pipeline_cmd(env, pipeline, prev_read_end,
				pipe_fd);
		if (last_pid == -1)
		{
			if (pipeline->next)
			{
				close(pipe_fd[READ_END]);
				close(pipe_fd[WRITE_END]);
			}
			return (1);
		}
		if (pipeline->next)
			prep_for_next_cmd(env, &prev_read_end, pipe_fd);
		else if (prev_read_end != -1)
			close(prev_read_end);
		pipeline = pipeline->next;
	}
	last_cmd_status = wait_for_children(env, last_pid, pipeline_count);
	printf("\n");
	return (last_cmd_status);
}

static t_list	*build_cmd_list(t_env *env, t_ast_node *node)
{
	t_list	*pipeline;

	(void)env;
	pipeline = NULL;
	if (!node)
		return (NULL);
	while (node && node->type == NODE_PIPE)
	{
		if (node->right)
			ft_lstadd_front(&pipeline, ft_lstnew(node->right));
		node = node->left;
	}
	if (node)
		ft_lstadd_front(&pipeline, ft_lstnew(node));
	return (pipeline);
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
