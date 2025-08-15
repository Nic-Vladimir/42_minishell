/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:52:57 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:18:21 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_env(t_env *env, t_ast_node *node)
{
	int		i;
	char	**envp;
	size_t	len;
	ssize_t	status;

	(void)node;
	i = 0;
	envp = get_envp_from_hashmap(env);
	while (envp[i])
	{
		len = ft_strlen(envp[i]);
		status = write(STDOUT_FILENO, envp[i], len);
		if (status == -1 || write(STDOUT_FILENO, "\n", 1) == -1)
		{
			free_envp(envp);
			return (1);
		}
		i++;
	}
	free_envp(envp);
	return (0);
}
