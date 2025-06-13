/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:20:38 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/13 16:06:57 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

void debug_print_heredoc_content(int read_fd)
{
    char buffer[4096];
    ssize_t bytes_read;
    int original_fd;
    int pipe_fds[2];
    
    // We need to preserve the content for the command execution
    // So we'll create a new pipe
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        return;
    }
    
    // Save the original fd to restore it later
    original_fd = dup(read_fd);
    if (original_fd == -1)
    {
        perror("dup");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return;
    }
    
    fprintf(stderr, "\n----- HEREDOC CONTENT START -----\n");
    
    // Read from the original fd and write to both stdout for debugging
    // and the new pipe for later command execution
    while ((bytes_read = read(read_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        // Null-terminate for printing
        buffer[bytes_read] = '\0';
        
        // Print to stderr for debugging
        fprintf(stderr, "%s", buffer);
        
        // Write to the new pipe for the command
        write(pipe_fds[1], buffer, bytes_read);
    }
    
    fprintf(stderr, "\n----- HEREDOC CONTENT END -----\n");
    
    // Close original and write end of new pipe
    close(read_fd);
    close(pipe_fds[1]);
    
    // Restore the read_fd with our new pipe's read end
    dup2(pipe_fds[0], original_fd);
    close(pipe_fds[0]);
}

int	execute_redirections(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	int	new_fd;
	int	status;

	new_fd = setup_redirection_fd(env, &node);
	if (new_fd == -1)
		return (1);
	if (node->type == NODE_REDIR_IN && in_fd != STDIN_FILENO)
		close(in_fd);
	if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND)
		&& out_fd != STDOUT_FILENO)
		close(out_fd);
	if (node->type == NODE_HEREDOC)
		//debug_print_heredoc_content(new_fd);
		status = execute_node(env, node->right, new_fd, out_fd);
	else
		status = execute_node(env, node->right, in_fd, new_fd);
	return (status);
}

// int	execute_redirections(t_env *env, t_ast_node *node, int in_fd,
//	int out_fd)
//{
//	int		new_fd;
//	int		status;
//	char	*delimiter;
//
//	new_fd = -1;
//	status = 0;
//	if (node->type == NODE_HEREDOC)
//	{
//		delimiter = node->args[0];
//		if (collect_heredoc(env, delimiter, &new_fd) == -1)
//			return (1);
//	}
//	if (node->type == NODE_REDIR_IN)
//		new_fd = open(node->args[0], O_RDONLY);
//	else if (node->type == NODE_REDIR_OUT)
//	{
//		new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//		node = handle_multiple_redirections(node);
//	}
//	else if (node->type == NODE_REDIR_APPEND)
//	{
//		new_fd = open(node->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
//		node = handle_multiple_redirections(node);
//	}
//	if (new_fd == -1 && node->type != NODE_HEREDOC)
//		return (1);
//	if (node->type == NODE_REDIR_IN && in_fd != STDIN_FILENO)
//		close(in_fd);
//	if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND)
//		&& out_fd != STDOUT_FILENO)
//		close(out_fd);
//	if (node->type == NODE_HEREDOC)
//		status = execute_node(env, node->right, new_fd, out_fd);
//	else
//		status = execute_node(env, node->right, in_fd, new_fd);
//	return (status);
//}
