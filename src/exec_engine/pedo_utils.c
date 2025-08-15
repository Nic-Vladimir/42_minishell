/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 08:54:24 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/15 15:31:47 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pedo.h"

static void	child_process(t_env *env, t_child_data *child_data, int in_fd,
		int out_fd)
{
	int	result;

	setup_child_fds(in_fd, out_fd);
	setup_child_signals(env, child_data->signal_behavior);
	result = child_data->func(child_data->data);
	if (child_data->cleanup)
		child_data->cleanup(child_data->data);
	exit(result);
}

static int	parent_process(pid_t pid, t_env *env, int in_fd, int out_fd)
{
	int	status;

	(void)env;
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	set_all_signals(MINI_MODE, env->sigenv);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (128 + WTERMSIG(status));
}

int	execute_in_child(t_env *env, t_child_data *child_data, int in_fd,
		int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		child_process(env, child_data, in_fd, out_fd);
	}
	return (parent_process(pid, env, in_fd, out_fd));
}
