/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:27:53 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:37:11 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <readline/readline.h>

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

static bool	herdoc_stop(char *line, char *delimiter)
{
	if (!line)
		return (true);
	if (ft_strcmp(line, delimiter) == 0)
		return (true);
	return (false);
}

static int	expanded_herdoc(char *expanded, ssize_t status, int write_fd,
		char *line)
{
	status = write(write_fd, expanded, ft_strlen(expanded));
	if (status == -1 || write(write_fd, "\n", 1) == -1)
	{
		perror("write failed");
		status = status_check(&write_fd, status, expanded, line);
	}
	return (status);
}

int	process_heredoc_input(t_env *env, char *delimiter, int write_fd)
{
	char	*line;
	char	*expanded;
	ssize_t	status;

	status = 0;
	while (1)
	{
		line = readline("> ");
		if (herdoc_stop(line, delimiter))
		{
			if (!line)
				ft_printf("delimited by end-of-file (wanted `%s')\n",
					delimiter);
			free(line);
			break ;
		}
		expanded = expand_var(env, line);
		if (expanded)
		{
			status = expanded_herdoc(expanded, status, write_fd, line);
			free(expanded);
		}
		free(line);
	}
	return (0);
}
