/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:27:53 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/23 22:56:01 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	status_check(int *pipe_fds, int status, char *expanded, char *line)
{
	if (status == -1)
	{
		free(expanded);
		free(line);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}
	return (0);
}

static int	process_heredoc_input(t_env *env, char *delimiter, int write_fd)
{
	char	*line;
	char	*expanded;
	ssize_t	status;

	status = 0;
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		expanded = expand_var(env, line);
		if (expanded)
		{
			status = write(write_fd, expanded, ft_strlen(expanded));
			if (status == -1 || write(write_fd, "\n", 1) == -1)
				status_check(NULL, status, expanded, line);
			free(expanded);
		}
		free(line);
	}
	return (status);
}

int	collect_heredoc(t_env *env, char *delimiter, int *write_fd)
{
	int		pipe_fds[2];
	ssize_t	status;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	status = process_heredoc_input(env, delimiter, pipe_fds[1]);
	close(pipe_fds[1]);
	*write_fd = pipe_fds[0];
	return (status);
}

// int	collect_heredoc(t_env *env, char *delimiter, int *write_fd)
//{
//	char	*line;
//	char	*expanded;
//	int		pipe_fds[2];
//	ssize_t	status;
//
//	status = 0;
//	if (pipe(pipe_fds) == -1)
//	{
//		perror("pipe failed");
//		return (-1);
//	}
//	while (1)
//	{
//		line = readline("heredoc> ");
//		if (!line || ft_strcmp(line, delimiter) == 0)
//		{
//			if (line)
//				free(line);
//			break ;
//		}
//		expanded = expand_var(env, line);
//		if (expanded)
//		{
//			status = write(pipe_fds[1], expanded, ft_strlen(expanded));
//			if (status == -1 || write(pipe_fds[1], "\n", 1) == -1)
//				status_check(pipe_fds, status, expanded, line);
//			free(expanded);
//		}
//		free(line);
//	}
//	close(pipe_fds[1]);
//	*write_fd = pipe_fds[0];
//	return (status);
//}
