/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/04 09:25:28 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

int		check_process_child_exit(t_env *env, int status);

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
		return (execute_exit(env, env->last_exit_code));
	return (1);
}

void	free_pipeline_list(t_list **pipeline)
{
	t_list	*tmp;

	while (*pipeline)
	{
		tmp = (*pipeline)->next;
		free(*pipeline);
		*pipeline = tmp;
	}
	*pipeline = NULL;
}

bool	cmd_is_builtin(t_env *env, t_ast_node *node)
{
	(void)env;
	return (ft_strncmp(node->args[0], "cd", 3) == 0 || ft_strncmp(node->args[0],
			"echo", 5) == 0 || ft_strncmp(node->args[0], "pwd", 4) == 0
		|| ft_strncmp(node->args[0], "export", 7) == 0
		|| ft_strncmp(node->args[0], "env", 4) == 0 || ft_strncmp(node->args[0],
			"unset", 6) == 0 || ft_strncmp(node->args[0], "exit", 6) == 0);
}

void	handle_execve(t_env *env, t_ast_node *node)
{
	char	**envp;
	char	*exec_path;

	envp = get_envp_from_hashmap(env);
	exec_path = find_executable(env, node->args[0]);
	if (exec_path == NULL)
	{
		perror("minishell");
		free_pipeline_list(&env->pipeline);
		free_ast(&env->root);
		free_env(env);
		free_envp(envp);
		exit(127);
	}
	if (execve(exec_path, node->args, envp) == -1)
	{
		ft_printf("minishell: %s: Is not an executable binary\n", exec_path);
		free_pipeline_list(&env->pipeline);
		free_ast(&env->root);
		free(exec_path);
		free_envp(envp);
		free_env(env);
		exit(127);
	}
}

int	execute_external_command(t_env *env, t_ast_node *node,
		t_execute_type exec_type)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (exec_type == EXIT)
		handle_execve(env, node);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			set_all_signals(NORMAL_MODE, env->sigenv);
			handle_execve(env, node);
		}
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		set_all_signals(MINI_MODE, env->sigenv);
		// write(STDOUT_FILENO, "\n", 1);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (128 + WTERMSIG(status));
	}
	return (status);
}

int	execute_command(t_env *env, t_ast_node *node, t_execute_type exec_type)
{
	int	status;

	if (cmd_is_builtin(env, node))
		status = execute_builtin_command(env, node);
	else
		status = execute_external_command(env, node, exec_type);
	if (env->pipeline != NULL)
	{
		free_pipeline_list(&env->pipeline);
		free_ast(&env->root);
		free_env(env);
	}
	return (status);
}

int	execute_logical_op(t_env *env, t_ast_node *node)
{
	int	left_status;
	int	right_status;

	left_status = execute(env, node->left, RETURN);
	env->last_exit_code = left_status;
	if (node->type == NODE_AND)
	{
		if (left_status == 0)
		{
			right_status = execute(env, node->right, RETURN);
			return (right_status);
		}
		return (left_status);
	}
	else if (node->type == NODE_OR)
	{
		if (left_status != 0)
		{
			right_status = execute(env, node->right, RETURN);
			return (right_status);
		}
		return (left_status);
	}
	return (1);
}

int	execute_group(t_env *env, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		// signals
		status = execute(env, node->group, EXIT);
		free_ast(&env->root);
		free_env(env);
		exit(status);
	}
	wait(&status);
	status = check_process_child_exit(env, status);
	return (status);
}

/*
int	redirect_input(t_env *env, t_ast *node)
{
	int	input_fd;
	int	original_stdin;
	int	status;

	input_fd = open()
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
*/

int	execute(t_env *env, t_ast_node *node, t_execute_type exec_type)
{
	int	status;

	if (!node)
		return (0);
	printf("[DEBUG] Executing node type: %d\n", node->type);
	if (node->type == NODE_CMD)
	{
		execute_command_expansion(env, node);
		status = execute_command(env, node, exec_type);
	}
	else if (node->type == NODE_PIPE)
	{
		printf("[DEBUG] Executing pipeline\n");
		status = execute_pipeline(env, node);
	}
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
	{
		printf("[DEBUG] Executing redirection, type: %d\n", node->type);
		status = execute_redirections(env, node);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
		status = execute_logical_op(env, node);
	else if (node->type == NODE_GROUP)
		status = execute_group(env, node);
	else
	{
		perror("Unknown node type in execution\n");
		return (1);
	}
	EXIT_SUCCESS;
	printf("[DEBUG] Execution completed with status: %d\n", status);
	return (status);
}

int	execute_ast(t_env *env, t_ast_node *root, t_execute_type exec_type)
{
	return (execute(env, root, exec_type));
}
