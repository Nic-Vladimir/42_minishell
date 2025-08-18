/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 11:09:06 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/18 11:18:39 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

pid_t	ft_getpid(void)
{
	int		fd;
	char	buf[32];
	pid_t	pid;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
		return (-1);
	if (read(fd, buf, sizeof(buf) - 1) <= 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	buf[sizeof(buf) - 1] = '\0';
	pid = ft_atoi(buf);
	return (pid);
}
