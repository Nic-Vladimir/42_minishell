/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:12:00 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	execute_builtin_command(t_env *env, t_ast_node *node)
{
	if (node->args[0] == NULL)
		return (1);
	if (ft_strncmp(node->args[0], "cd", 3) == 0)
		return (execute_cd(env, node));
	if (ft_strncmp(node->args[0], "echo", 5) == 0)
		return (execute_echo(env, node));
	if (ft_strncmp(node->args[0], "pwd", 4) == 0)
		return (execute_pwd(env, node));
	if (ft_strncmp(node->args[0], "export", 7) == 0)
		return (execute_export(env, node));
	if (ft_strncmp(node->args[0], "env", 4) == 0)
		return (execute_env(env, node));
	if (ft_strncmp(node->args[0], "unset", 6) == 0)
		return (execute_unset(env, node));
	if (ft_strncmp(node->args[0], "exit", 6) == 0)
		return (execute_exit(env, node, g_sig));
	return (1);
}

t_list	*build_cmd_list(t_env *env, t_ast_node *node)
{
	t_list	*pipeline;

	(void)env;
	pipeline = NULL;
	if (!node)
		return (NULL);
	while (node && node->type == NODE_PIPE)
	{
		if (node->right)
			ft_lstadd_front(&pipeline, ft_lstnew(node->right));
		node = node->left;
	}
	if (node)
		ft_lstadd_front(&pipeline, ft_lstnew(node));
	return (pipeline);
}

void	prep_for_next_cmd(t_env *env, int *prev_read_end, int *pipe_fd)
{
	(void)env;
	if (*prev_read_end != -1)
		close(*prev_read_end);
	close(pipe_fd[WRITE_END]);
	*prev_read_end = pipe_fd[READ_END];
}

bool	cmd_is_builtin(t_env *env, t_ast_node *node)
{
	(void)env;
	if (!node->args[0])
		return (false);
	return (ft_strncmp(node->args[0], "cd", 3) == 0 || ft_strncmp(node->args[0],
			"echo", 5) == 0 || ft_strncmp(node->args[0], "pwd", 4) == 0
		|| ft_strncmp(node->args[0], "export", 7) == 0
		|| ft_strncmp(node->args[0], "env", 4) == 0 || ft_strncmp(node->args[0],
			"unset", 6) == 0 || ft_strncmp(node->args[0], "exit", 6) == 0);
}

int	execute(t_env *env, t_ast_node *node, t_execute_type exec_type)
{
	int	status;

	if (!node)
		return (0);
	status = execute_node_by_type(env, node, exec_type);
	return (status);
}
