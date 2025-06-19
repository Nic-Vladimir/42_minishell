/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:02:04 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/17 15:05:34 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/pedo.h"

void	herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter)
{
	hd->env = env;
	hd->delimiter = delimiter;
}

static int	heredoc_child_func(void *data)
{
	t_heredoc_data	*hd;

	hd = (t_heredoc_data *)data;
	return (process_heredoc_input(hd->env, hd->delimiter, hd->write_fd));
}

int	collect_heredoc(t_env *env, char *delimiter, int *write_fd)
{
	int				pipe_fds[2];
	int				result;
	t_heredoc_data	hd_data;
	t_child_data	child_data;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	init_structs(&hd_data, &child_data);
	herdoc_linker(&hd_data, env, delimiter);
	hd_data.write_fd = pipe_fds[1];
	child_linker(&child_data, &hd_data, heredoc_child_func);
	result = execute_in_child(env, &child_data, STDIN_FILENO, STDOUT_FILENO);
	close(pipe_fds[1]);
	if (result != 0)
	{
		close(pipe_fds[0]);
		return (-1);
	}
	*write_fd = pipe_fds[0];
	return (0);
}
