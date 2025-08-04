/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:02:04 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/04 09:30:04 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include "../../inc/pedo.h"
#include <readline/readline.h>
#include <fcntl.h>
#include <sys/wait.h>

void	heredoc_child_cleanup(t_env *env)
{
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


void	herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter)
{
	hd->env = env;
	hd->delimiter = delimiter;
}

// static int	heredoc_child_func(void *data)
// {
// 	t_heredoc_data	*hd;

// 	hd = (t_heredoc_data *)data;
// 	return (process_heredoc_input(hd->env, hd->delimiter, hd->write_fd));
// }

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

static int	backup_stdio(int *orig_stdin, int *orig_stdout)
{
	*orig_stdin = dup(STDIN_FILENO);
	*orig_stdout = dup(STDOUT_FILENO);
	
	if (*orig_stdin == -1 || *orig_stdout == -1)
	{
		printf("[DEBUG] Child: Failed to backup stdio\n");
		if (*orig_stdin != -1)
			close(*orig_stdin);
		if (*orig_stdout != -1)
			close(*orig_stdout);
		return (-1);
	}
	return (0);
}

static int	open_and_redirect_tty(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
	{
		printf("[DEBUG] Child: Failed to open /dev/tty\n");
		return (-1);
	}
	
	printf("[DEBUG] Child: Opened /dev/tty, fd=%d\n", tty_fd);
	
	if (dup2(tty_fd, STDIN_FILENO) == -1 || dup2(tty_fd, STDOUT_FILENO) == -1)
	{
		printf("[DEBUG] Child: Failed to redirect stdio to /dev/tty\n");
		close(tty_fd);
		return (-1);
	}
	
	close(tty_fd);
	return (0);
}

static void	cleanup_on_error(int orig_stdin, int orig_stdout)
{
	dup2(orig_stdin, STDIN_FILENO);
	dup2(orig_stdout, STDOUT_FILENO);
	close(orig_stdin);
	close(orig_stdout);
}

static int	setup_tty_redirection(int *orig_stdin, int *orig_stdout)
{
	printf("[DEBUG] Child: Setting up TTY redirection\n");
	
	if (backup_stdio(orig_stdin, orig_stdout) == -1)
		return (-1);
	
	if (open_and_redirect_tty() == -1)
	{
		cleanup_on_error(*orig_stdin, *orig_stdout);
		return (-1);
	}
	
	return (0);
}

static void	restore_stdio(int orig_stdin, int orig_stdout)
{
	printf("[DEBUG] Child: Restoring original stdio\n");
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
	printf("[DEBUG] Child: heredoc_child_func started\n");
	
	if (setup_tty_redirection(&orig_stdin, &orig_stdout) == -1)
		return (-1);
	
	printf("[DEBUG] Child: Processing heredoc input\n");
	result = process_heredoc_input(hd->env, hd->delimiter, hd->write_fd);
	printf("[DEBUG] Child: heredoc processing result: %d\n", result);
	
	restore_stdio(orig_stdin, orig_stdout);
	
	return (result);
}

int	collect_heredoc(t_env *env, char *delimiter, int *write_fd)
{
	int				pipe_fds[2];
	int				result;
	t_heredoc_data	hd_data;
	t_child_data	child_data;

	printf("[DEBUG] collect_heredoc called with delimiter: '%s'\n", delimiter);
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	printf("[DEBUG] Created pipe: read_fd=%d, write_fd=%d\n", pipe_fds[0], pipe_fds[1]);
	
	init_structs(&hd_data, &child_data);
	herdoc_linker(&hd_data, env, delimiter);
	hd_data.write_fd = pipe_fds[1];
	child_linker(&child_data, &hd_data, heredoc_child_func);
	child_data.cleanup = heredoc_cleanup;
	printf("[DEBUG] About to execute heredoc in child process\n");
	result = execute_in_child(env, &child_data, STDIN_FILENO, STDOUT_FILENO);
	printf("[DEBUG] Child process returned: %d\n", result);
	close(pipe_fds[1]);

	if (result != 0)
	{
		printf("[DEBUG] Child failed, closing read end\n");
		close(pipe_fds[0]);
		return (-1);
	}
	*write_fd = pipe_fds[0];
	printf("[DEBUG] Heredoc collection successful, returning read_fd=%d\n", *write_fd);
	return (0);
}

