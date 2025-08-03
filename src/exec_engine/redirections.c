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
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/*
static int	handle_multiple_redirections(t_ast_node **node)
{
 int	fd;
 int	prev_fd;

 prev_fd = -1;
 while ((*node)->right->type == NODE_REDIR_OUT
		|| (*node)->right->type == NODE_REDIR_APPEND)
 {
		*node = (*node)->right;
		if ((*node)->type == NODE_REDIR_OUT)
			fd = open((*node)->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open((*node)->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			if (prev_fd != -1)
				close(prev_fd);
			return (-1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		prev_fd = fd;
 }
 return (prev_fd);
}

static int	setup_redirection_fd(t_env *env, t_ast_node *node)
{
	int		new_fd;
	char	*delimiter;

	new_fd = -1;
	if (node->type == NODE_HEREDOC)
	{
		delimiter = node->args[0];
		if (collect_heredoc(env, delimiter, &new_fd) == -1)
			return (-1);
	}
	else if (node->type == NODE_REDIR_IN)
		new_fd = open(node->args[0], O_RDONLY);
	else if (node->type == NODE_REDIR_OUT)
		new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->type == NODE_REDIR_APPEND)
		new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (new_fd);
}
*/
int	append_output(t_env *env, t_ast_node *node)
{
	int	out_fd;
	int	og_out_fd;
	int	status;

	status = EXIT_FAILURE;
	out_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (out_fd == -1)
		return (status);
	og_out_fd = dup(STDOUT_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	status = execute(env, node->right, RETURN);
	dup2(og_out_fd, STDIN_FILENO);
	close(og_out_fd);
	return (status);
}

int	redirect_output(t_env *env, t_ast_node *node)
{
	int	out_fd;
	int	og_out_fd;
	int	status;

	status = EXIT_FAILURE;
	out_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd == -1)
		return (status);
	og_out_fd = dup(STDOUT_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	status = execute(env, node->right, RETURN);
	dup2(og_out_fd, STDIN_FILENO);
	close(og_out_fd);
	return (status);
}

int	redirect_input(t_env *env, t_ast_node *node)
{
	int		in_fd;
	int		og_in_fd;
	int		status;
	char	*delimiter;

	status = EXIT_FAILURE;
	if (node->type == NODE_HEREDOC)
	{
		delimiter = node->args[0];
		if (collect_heredoc(env, delimiter, &in_fd) == -1)
			return (status);
	}
	else if (node->type == NODE_REDIR_IN)
		in_fd = open(node->args[0], O_RDONLY);
	if (in_fd == -1)
		return (status);
	og_in_fd = dup(STDIN_FILENO);
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
	status = execute(env, node->right, RETURN);
	dup2(og_in_fd, STDIN_FILENO);
	close(og_in_fd);
	return (status);
}

int	execute_redirections(t_env *env, t_ast_node *node)
{
	int	status;

	status = EXIT_FAILURE;
	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
		status = redirect_input(env, node);
	else if (node->type == NODE_REDIR_OUT)
		status = redirect_output(env, node);
	else if (node->type == NODE_REDIR_APPEND)
		status = append_output(env, node);
	return (status);
}
