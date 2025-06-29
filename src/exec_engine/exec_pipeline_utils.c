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

void	free_pipeline(t_pipeline *pl)
{
	free(pl->commands);
	free(pl->pipefds);
	free(pl->pids);
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

int	init_pipeline(t_ast_node *node, t_pipeline *pl)
{
	int			count;
	t_ast_node	**commands;
	int			*pipefds;
	pid_t		*pids;

	commands = collect_pipeline_commands(node, &count);
	if (!commands)
		return (perror("malloc"), 1);
	pipefds = malloc(sizeof(int) * 2 * (count - 1));
	pids = malloc(sizeof(pid_t) * count);
	if (!pipefds || !pids)
	{
		free(commands);
		free(pipefds);
		free(pids);
		return (perror("malloc"), 1);
	}
	pl->commands = commands;
	pl->pipefds = pipefds;
	pl->pids = pids;
	pl->count = count;
	return (0);
}

int	setup_pipes(t_pipeline *pl)
{
	int	count;
	int	*pipefds;
	int	i;

	i = 0;
	count = pl->count;
	pipefds = pl->pipefds;
	while (i < count - 1)
	{
		if (pipe(pipefds + i * 2) < 0)
			return (perror("pipe"), 1);
		i++;
	}
	return (0);
}

void	handle_child(t_env *env, t_pipeline *pl, int i, int in_fd, int out_fd)
{
	int	count;
	int	*pipefds;
	int	child_in_fd;
	int	child_out_fd;

	count = pl->count;
	pipefds = pl->pipefds;
	child_in_fd = (i == 0) ? in_fd : pipefds[(i - 1) * 2];
	child_out_fd = (i == count - 1) ? out_fd : pipefds[i * 2 + 1];
	signal(SIGINT, SIG_DFL);
	if (child_in_fd != STDIN_FILENO)
		dup2(child_in_fd, STDIN_FILENO);
	if (child_out_fd != STDOUT_FILENO)
		dup2(child_out_fd, STDOUT_FILENO);
	for (int j = 0; j < count - 1; j++)
	{
		if (j != i - 1)
			close(pipefds[j * 2]);
		if (j != i)
			close(pipefds[j * 2 + 1]);
	}
	execute_child(env, pl->commands[i], child_in_fd, child_out_fd);
	exit(1);
}

int	manage_children(t_env *env, t_pipeline *pl, int in_fd, int out_fd)
{
	int		count;
	int		*pipefds;
	pid_t	*pids;
	int		i;

	i = 0;
	count = pl->count;
	pipefds = pl->pipefds;
	pids = pl->pids;
	while (i < count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (perror("fork"), 1);
		if (pids[i] == 0)
			handle_child(env, pl, i, in_fd, out_fd);
		i++;
	}
	i = 0;
	while (i < count - 1)
	{
		close(pipefds[i * 2]);
		close(pipefds[i++ * 2 + 1]);
	}
	return (0);
}
