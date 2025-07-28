/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:20:38 by vnicoles          #+#    #+#             */
/*   Updated: 2025/07/28 14:30:03 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_ast_node	*handle_multiple_redirections(t_ast_node *node)
{
	int	temp_fd;

	while (node->right->type == NODE_REDIR_OUT
		|| node->right->type == NODE_REDIR_APPEND)
	{
		node = node->right;
		if (node->type == NODE_REDIR_OUT)
			temp_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			temp_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		close(temp_fd);
	}
	return (node);
}

static int	setup_redirection_fd(t_env *env, t_ast_node **node)
{
	int		new_fd;
	char	*delimiter;

	new_fd = -1;
	if ((*node)->type == NODE_HEREDOC)
	{
		delimiter = (*node)->args[0];
		if (collect_heredoc(env, delimiter, &new_fd) == -1)
			return (-1);
	}
	else if ((*node)->type == NODE_REDIR_IN)
		new_fd = open((*node)->args[0], O_RDONLY);
	else if ((*node)->type == NODE_REDIR_OUT)
	{
		new_fd = open((*node)->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		*node = handle_multiple_redirections(*node);
	}
	else if ((*node)->type == NODE_REDIR_APPEND)
	{
		new_fd = open((*node)->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		*node = handle_multiple_redirections(*node);
	}
	return (new_fd);
}

int	execute_redirections(t_env *env, t_ast_node *node)
{
	int	new_fd;
	int	status;

	new_fd = setup_redirection_fd(env, &node);
	if (new_fd == -1)
		return (1);
	/*
	if (node->type == NODE_REDIR_IN && in_fd != STDIN_FILENO)
		close(in_fd);
	if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND)
		&& out_fd != STDOUT_FILENO)
		close(out_fd);
	if (node->type == NODE_HEREDOC)
		status = execute_node(env, node->right, new_fd, out_fd);
	else
		*/
	status = execute(env, node->right, RETURN);
	return (status);
}
