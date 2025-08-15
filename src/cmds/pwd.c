/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:05:11 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:25:28 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_pwd(t_env *env, t_ast_node *node)
{
	char	*cwd;
	ssize_t	len;
	ssize_t	status;

	status = 0;
	cwd = get_env_value(env, "PWD");
	if (node->args[1])
	{
		ft_printf("pwd: too many arguments\n");
		return (1);
	}
	else if (*cwd)
	{
		len = ft_strlen(cwd);
		status = write(STDOUT_FILENO, cwd, len);
		if (status == -1 || write(STDOUT_FILENO, "\n", 1) == -1)
			return (1);
		return (0);
	}
	else
	{
		perror(YELLOW "getcwd err" RESET);
		return (1);
	}
}
