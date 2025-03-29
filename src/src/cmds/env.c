/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 03:52:57 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/26 21:01:52 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_env(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	int		i;
	char	**envp;
    size_t  len;

	(void)node;
	(void)in_fd;
	i = 0;
	envp = get_envp_from_hashmap(env);
	while (envp[i])
	{
        len = ft_strlen(envp[i]);
        write(out_fd, envp[i], len);
        write(out_fd, "\n", 1);
        i++;
	}
	return 0;
}
