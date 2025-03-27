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

	(void)node;
	(void)in_fd;
	(void)out_fd;
	i = 0;
	envp = get_envp_from_hashmap(env);
	while (envp[i])
	{
    	printf("%s\n", envp[i++]);
	}
	return 0;
}
