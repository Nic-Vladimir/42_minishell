/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/24 11:59:07 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int			execute_builtin_command(t_env *env, t_ast_node *node, int in_fd,
				int out_fd);
int			execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd);
static int	execute_pipeline(t_env *env, t_ast_node *node, int in_fd,
				int out_fd);
int			execute_redirections(t_env *env, t_ast_node *node, int in_fd,
				int out_fd);
static int	execute_logical_op(t_env *env, t_ast_node *node, int in_fd,
				int out_fd);
static int	execute_group(t_env *env, t_ast_node *node, int in_fd, int out_fd);

// int	execute_command_expansion(t_env *env, t_ast_node *node, int in_fd,
//		int out_fd)
//{
//	int		i;
//	char	*temp;
//	char	*expanded;
//	char	*args_str;
//	char	**args_list;
//	char	**temp_args;
//
//	/*
//	//TODO: Debugging
//	printf("Node type: %d\n", node->type);
//	printf("Node args: %p\n", (void*)node->args);
//	if (node->args) {
//		for (int j = 0; node->args[j] != NULL; j++) {
//			printf("Args Before execute_command [%d]: %s\n", j, node->args[j]);
//		}
//	}
//	*/
//	i = 0;
//	while (node->args[i] != NULL)
//	{
//		if (node->arg_types[i] == TOK_WORD && ft_strchr(node->args[i], '*'))
//		{
//			temp = node->args[i];
//			args_str = expand_wildcard(temp);
//			// printf("args_str: %s\n", args_str);
//			if (!node->args[i])
//				return (1);
//			args_list = ft_split(args_str, ' ');
//			free(args_str);
//			temp_args = copy_args(node, i, args_list);
//			free_list(node->args);
//			node->args = temp_args;
//			ft_free_split(args_list);
//		}
//		else if (node->arg_types[i] == TOK_WORD
//			|| node->arg_types[i] == TOK_DBQ_BLOCK)
//		{
//			temp = node->args[i];
//			expanded = expand_var(env, temp);
//			node->args[i] = ft_strdup(expanded);
//			free(expanded);
//			free(temp);
//			if (!node->args[i])
//				return (1);
//		}
//		i++;
//	}
//	return (execute_builtin_command(env, node, in_fd, out_fd));
//}

int	execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	if (!node)
		return (0);
	if (node->type == NODE_CMD)
		return (execute_command_expansion(env, node, in_fd, out_fd));
	else if (node->type == NODE_PIPE)
		return (execute_pipeline(env, node, in_fd, out_fd));
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
		return (execute_redirections(env, node, in_fd, out_fd));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (execute_logical_op(env, node, in_fd, out_fd));
	else if (node->type == NODE_GROUP)
		return (execute_group(env, node, in_fd, out_fd));
	else
	{
		fprintf(stderr, "Unknown node type in execution\n");
		return (1);
	}
}

// int	execute_node(t_env *env, t_ast_node *node, int in_fd, int out_fd)
//{
//	if (!node)
//		return (0);
//	switch (node->type)
//	{
//	case NODE_CMD:
//		return (execute_command_expansion(env, node, in_fd, out_fd));
//	case NODE_PIPE:
//		return (execute_pipeline(env, node, in_fd, out_fd));
//	case NODE_REDIR_IN:
//	case NODE_REDIR_OUT:
//	case NODE_REDIR_APPEND:
//	case NODE_HEREDOC:
//		return (execute_redirections(env, node, in_fd, out_fd));
//	case NODE_AND:
//	case NODE_OR:
//		return (execute_logical_op(env, node, in_fd, out_fd));
//	case NODE_GROUP:
//		return (execute_group(env, node, in_fd, out_fd));
//	default:
//		fprintf(stderr, "Unknown node type in execution\n");
//		return (1);
//	}
//}

// int	execute_command(t_env *env, t_ast_node *node, int in_fd, int out_fd)
//{
//	pid_t	pid;
//	char	**envp;
//	char	*exec_path;
//	int		status;
//
//	pid = fork();
//	if (pid == 0)
//	{
//		if (in_fd != STDIN_FILENO)
//		{
//			dup2(in_fd, STDIN_FILENO);
//			close(in_fd);
//		}
//		if (out_fd != STDOUT_FILENO)
//		{
//			dup2(out_fd, STDOUT_FILENO);
//			close(out_fd);
//		}
//		envp = get_envp_from_hashmap(env);
//		exec_path = find_executable(env, node->args[0]);
//		// printf("Exec_path: %s", exec_path);
//		if (exec_path == NULL)
//		{
//			perror("Not valid exec_path");
//			free_ast(node);
//			free_env(env);
//			free_envp(envp);
//			exit(127);
//		}
//		/*
//		//TODO: Remove this debug print
//		printf("\nArgs: [%s]", exec_path);
//		for (int i = 0; node->args[i]; i++)
//			printf(" [%s]", node->args[i]);
//		printf("\n");
//		*/
//		// printf("Child: in_fd: %d, exec_path: %s\n", in_fd, exec_path);
//		if (execve(exec_path, node->args, envp) == -1)
//			handle_execve_error(node, envp);
//		fprintf(stderr, "Command not found: %s\n", node->args[0]);
//		free_envp(envp);
//		exit(127);
//	}
//	waitpid(pid, &status, 0);
//	if (in_fd != STDIN_FILENO)
//		close(in_fd);
//	if (out_fd != STDOUT_FILENO)
//		close(out_fd);
//	if (WIFEXITED(status))
//		return (WEXITSTATUS(status));
//	else
//		return (1);
//}

static int	execute_pipeline(t_env *env, t_ast_node *node, int in_fd,
		int out_fd)
{
	int	pipefd[2];
	int	left_status;
	int	right_status;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	left_status = execute_node(env, node->left, in_fd, pipefd[1]);
	(void)left_status;
	close(pipefd[1]);
	right_status = execute_node(env, node->right, pipefd[0], out_fd);
	close(pipefd[0]);
	env->last_exit_code = right_status;
	return (right_status);
}

static int	execute_logical_op(t_env *env, t_ast_node *node, int in_fd,
		int out_fd)
{
	int	left_status;
	int	right_status;

	left_status = execute_node(env, node->left, in_fd, out_fd);
	*&env->last_exit_code = left_status;
	if ((node->type == NODE_AND && left_status == 0) || (node->type == NODE_OR
			&& left_status != 0))
	{
		right_status = execute_node(env, node->right, in_fd, out_fd);
		*&env->last_exit_code = right_status;
		return (right_status);
	}
	return (left_status);
}

static int	execute_group(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	return (execute_node(env, node->left, in_fd, out_fd));
}

int	execute_ast(t_env *env, t_ast_node *root)
{
	return (execute_node(env, root, STDIN_FILENO, STDOUT_FILENO));
}
