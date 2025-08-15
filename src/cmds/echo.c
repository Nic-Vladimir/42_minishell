/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:04:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:18:15 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

ssize_t	write_to_fd(t_ast_node *node, int i)
{
	ssize_t	status;

	status = 0;
	while (node->args[i])
	{
		status = write(STDOUT_FILENO, node->args[i], ft_strlen(node->args[i]));
		if (status == -1)
			return (1);
		if (node->args[i + 1])
		{
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (1);
		}
		i++;
	}
	return (status);
}

int	execute_echo(t_env *env, t_ast_node *node)
{
	int	is_flag;
	int	i;

	(void)env;
	i = 1;
	is_flag = 0;
	if (node->args[i] && ft_strcmp(node->args[i], "-n") == 0)
	{
		is_flag = 1;
		i++;
	}
	if (write_to_fd(node, i) == -1)
		return (-1);
	if (!is_flag)
	{
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (1);
	}
	return (0);
}
