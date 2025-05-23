/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:52:57 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/21 17:10:51 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_env(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	int		i;
	char	**envp;
	size_t	len;
	ssize_t	status;

	(void)node;
	(void)in_fd;
	i = 0;
	envp = get_envp_from_hashmap(env);
	while (envp[i])
	{
		len = ft_strlen(envp[i]);
		status = write(out_fd, envp[i], len);
		if (status == -1 || write(out_fd, "\n", 1) == -1)
		{
			free_envp(envp);
			return (1);
		}
		i++;
	}
	free_envp(envp);
	return (0);
}
