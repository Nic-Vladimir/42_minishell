/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:20:38 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/03 16:13:02 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

	fprintf(stderr, "DEBUG: append_output() called with file: %s\n", 
		node->args[0] ? node->args[0] : "NULL");
	status = EXIT_FAILURE;
	out_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	fprintf(stderr, "DEBUG: append_output() open() returned fd: %d\n", out_fd);
	if (out_fd == -1)
	{
		fprintf(stderr, "DEBUG: append_output() failed to open file: %s\n", 
			node->args[0]);
		return (status);
	}
	fprintf(stderr, "DEBUG: append_output() duplicating stdout, original fd: %d\n", 
		STDOUT_FILENO);
	og_out_fd = dup(STDOUT_FILENO);
	fprintf(stderr, "DEBUG: append_output() dup() returned fd: %d\n", og_out_fd);
	dup2(out_fd, STDOUT_FILENO);
	fprintf(stderr, "DEBUG: append_output() redirected stdout to fd: %d\n", out_fd);
	close(out_fd);
	fprintf(stderr, "DEBUG: append_output() calling execute() on right node\n");
	status = execute(env, node->right, RETURN);
	fprintf(stderr, "DEBUG: append_output() execute() returned status: %d\n", status);
	dup2(og_out_fd, STDOUT_FILENO);
	fprintf(stderr, "DEBUG: append_output() restored stdout\n");
	close(og_out_fd);
	fprintf(stderr, "DEBUG: append_output() finished, returning status: %d\n", status);
	return (status);
}

int	redirect_output(t_env *env, t_ast_node *node)
{
	int	out_fd;
	int	og_out_fd;
	int	status;

	fprintf(stderr, "DEBUG: redirect_output() called with file: %s\n", 
		node->args[0] ? node->args[0] : "NULL");
	status = EXIT_FAILURE;
	out_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	fprintf(stderr, "DEBUG: redirect_output() open() returned fd: %d\n", out_fd);
	if (out_fd == -1)
	{
		fprintf(stderr, "DEBUG: redirect_output() failed to open file: %s\n", 
			node->args[0]);
		return (status);
	}
	fprintf(stderr, "DEBUG: redirect_output() duplicating stdout, original fd: %d\n", 
		STDOUT_FILENO);
	og_out_fd = dup(STDOUT_FILENO);
	fprintf(stderr, "DEBUG: redirect_output() dup() returned fd: %d\n", og_out_fd);
	dup2(out_fd, STDOUT_FILENO);
	fprintf(stderr, "DEBUG: redirect_output() redirected stdout to fd: %d\n", out_fd);
	close(out_fd);
	fprintf(stderr, "DEBUG: redirect_output() calling execute() on right node\n");
	status = execute(env, node->right, RETURN);
	fprintf(stderr, "DEBUG: redirect_output() execute() returned status: %d\n", status);
	dup2(og_out_fd, STDOUT_FILENO);
	fprintf(stderr, "DEBUG: redirect_output() restored stdout\n");
	close(og_out_fd);
	fprintf(stderr, "DEBUG: redirect_output() finished, returning status: %d\n", status);
	return (status);
}

int	redirect_input(t_env *env, t_ast_node *node)
{
	int		in_fd;
	int		og_in_fd;
	int		status;
	char	*delimiter;

	fprintf(stderr, "DEBUG: redirect_input() called with node type: %d\n", node->type);
	status = EXIT_FAILURE;
	if (node->type == NODE_HEREDOC)
	{
		delimiter = node->args[0];
		fprintf(stderr, "DEBUG: redirect_input() processing heredoc with delimiter: %s\n", 
			delimiter ? delimiter : "NULL");
		if (collect_heredoc(env, delimiter, &in_fd) == -1)
		{
			fprintf(stderr, "DEBUG: redirect_input() collect_heredoc() failed\n");
			return (status);
		}
		fprintf(stderr, "DEBUG: redirect_input() collect_heredoc() returned fd: %d\n", in_fd);
	}
	else if (node->type == NODE_REDIR_IN)
	{
		fprintf(stderr, "DEBUG: redirect_input() opening file: %s\n", 
			node->args[0] ? node->args[0] : "NULL");
		in_fd = open(node->args[0], O_RDONLY);
		fprintf(stderr, "DEBUG: redirect_input() open() returned fd: %d\n", in_fd);
	}
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
	{
		fprintf(stderr, "DEBUG: Redirecting output to %s\n", node->args[0]);
		status = redirect_output(env, node);
	}
	else if (node->type == NODE_REDIR_APPEND)
	{
		fprintf(stderr, "DEBUG: Appending output to %s\n", node->args[0]);
		status = append_output(env, node);
	}
	return (status);
}
