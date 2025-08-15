/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:02:04 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/15 15:27:39 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pedo.h"

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
		if (env->pipeline)
			free_pipeline_list(&env->pipeline);
		free_env(env);
	}
}

static int	redirect_stdio_to_tty(int *orig_stdin, int *orig_stdout)
{
	int	tty_fd;

	*orig_stdin = dup(STDIN_FILENO);
	*orig_stdout = dup(STDOUT_FILENO);
	if (*orig_stdin == -1 || *orig_stdout == -1)
		return (-1);
	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
		return (-1);
	if (dup2(tty_fd, STDIN_FILENO) == -1 || dup2(tty_fd, STDOUT_FILENO) == -1)
	{
		close(tty_fd);
		return (-1);
	}
	close(tty_fd);
	return (0);
}

static void	restore_stdio(int orig_stdin, int orig_stdout)
{
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	close(orig_stdin);
	close(orig_stdout);
}

static int	heredoc_child_func(void *data)
{
	t_heredoc_data	*hd;
	int				orig_stdin;
	int				orig_stdout;
	int				result;

	hd = (t_heredoc_data *)data;
	if (redirect_stdio_to_tty(&orig_stdin, &orig_stdout) == -1)
		return (-1);
	result = process_heredoc_input(hd->env, hd->delimiter, hd->write_fd);
	restore_stdio(orig_stdin, orig_stdout);
	return (result);
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
	child_data.cleanup = heredoc_cleanup;
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
