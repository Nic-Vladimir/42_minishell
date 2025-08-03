/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:02:04 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/03 20:49:05 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include "../../inc/pedo.h"

void	heredoc_child_cleanup(t_env *env)
{
	if (env)
	{
		if (env->root)
			free_ast(&env->root);
		if (env->tokenizer)
			free_tokens(env->tokenizer);
		free_env(env);
	}
}

static void	heredoc_cleanup(void *data)
{
	t_heredoc_data	*hd;
	t_env			*env;

	hd = (t_heredoc_data *)data;
	env = hd->env;
	if (env)
	{
		if (env->root)
			free_ast(&env->root);
		if (env->tokenizer)
			free_tokens(env->tokenizer);
		free_env(env);
	}
}

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
	int				tty_fd;
	t_heredoc_data	hd_data;
	t_child_data	child_data;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
	{
		perror("Failed to open /dev/tty");
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	init_structs(&hd_data, &child_data);
	herdoc_linker(&hd_data, env, delimiter);
	hd_data.write_fd = pipe_fds[1];
	child_linker(&child_data, &hd_data, heredoc_child_func);
	child_data.cleanup = heredoc_cleanup;
	result = execute_in_child(env, &child_data, tty_fd, STDOUT_FILENO);
	close(pipe_fds[1]);
	close(tty_fd);
	if (result != 0)
	{
		close(pipe_fds[0]);
		return (-1);
	}
	*write_fd = pipe_fds[0];
	return (result);
}
