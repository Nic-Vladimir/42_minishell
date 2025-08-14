/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:34:17 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/14 10:44:22 by mgavornik        ###   ########.fr       */
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

pid_t	execute_one_pipeline_cmd(t_env *env, t_list *pipeline,
		int prev_read_end, int *pipe_fd)
{
	pid_t		pid;
	// t_ast_node	*node;
	int			status;

	status = EXIT_FAILURE;
	// int			dummyfd;
	// dummyfd = open("/dev/null", O_WRONLY);
	if (!pipeline || !pipeline->content)
		return (-1);
	// node = (t_ast_node *)pipeline->content;
	pid = fork();
	fprintf(stderr, "[PID]execute_one_pipeline_cmd(): %d\n", pid);
	if (pid != 0)
	{
		return (pid);
	}
	setup_child_signals(env, CHILD_SIG_CUSTOM);
	if (pipe_fd[READ_END] != -1)
	{
		close(pipe_fd[READ_END]);
	}
	setup_child_fds(prev_read_end, pipe_fd[WRITE_END]);
	/*
	if (dummyfd != -1)
	{
		dup2(dummyfd, STDERR_FILENO);
		close(dummyfd);
	}
	*/
	status = execute(env, (t_ast_node *)pipeline->content, EXIT);
	comprehensive_cleanup(&env);
	exit(status);
}

void	prep_for_next_cmd(t_env *env, int *prev_read_end, int *pipe_fd)
{
	(void)env;
	if (*prev_read_end != -1)
		close(*prev_read_end);
	close(pipe_fd[WRITE_END]);
	*prev_read_end = pipe_fd[READ_END];
}

int	wait_for_children(t_env *env, pid_t last_pid, int pipeline_count)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_status;

	last_cmd_status = 0;
	while (pipeline_count--)
	{
		child_pid = wait(&status);
		if (child_pid == last_pid)
			last_cmd_status = check_process_child_exit(env, status);
		else
			check_process_child_exit(env, status);
	}
	return (last_cmd_status);
}
static int setup_pipe(t_list *pipeline, int *pipe_fd)
{
    if (pipeline->next)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return 1;
        }
    }
    else
    {
        pipe_fd[READ_END] = -1;
        pipe_fd[WRITE_END] = -1;
    }
    return 0;
}
static int handle_pipeline_error(t_list *pipeline, int *pipe_fd)
{
    if (pipeline->next)
    {
        close(pipe_fd[READ_END]);
        close(pipe_fd[WRITE_END]);
    }
    return 1;
}
static int process_pipeline_cmd(
    t_env *env, t_list *pipeline, int *prev_read_end, pid_t *last_pid)
{
    int pipe_fd[2];
    if (setup_pipe(pipeline, pipe_fd))
        return 1;
    *last_pid = execute_one_pipeline_cmd(env, pipeline, *prev_read_end, pipe_fd);
    if (*last_pid == -1)
        return handle_pipeline_error(pipeline, pipe_fd);
    if (pipeline->next)
        prep_for_next_cmd(env, prev_read_end, pipe_fd);
    else if (*prev_read_end != -1)
        close(*prev_read_end);
    return 0;
}
int execute_pipeline_cmds(t_env *env, t_list *pipeline)
{
    int prev_read_end = -1;
    pid_t last_pid = 0;
    int pipeline_count, last_cmd_status;
    if (!pipeline)
        return 1;
    pipeline_count = ft_lstsize(pipeline);
    env->pipeline = pipeline;
    t_list *current = pipeline;
    while (current)
    {
        if (process_pipeline_cmd(env, current, &prev_read_end, &last_pid))
            return 1;
        current = current->next;
    }
    last_cmd_status = wait_for_children(env, last_pid, pipeline_count);
    printf("\n");
    return last_cmd_status;
}
// int	execute_pipeline_cmds(t_env *env, t_list *pipeline)
// {
// 	int		pipe_fd[2];
// 	int		prev_read_end;
// 	pid_t	last_pid;
// 	int		last_cmd_status;
// 	int		pipeline_count;

// 	if (!pipeline)
// 		return (1);
// 	pipeline_count = ft_lstsize(pipeline);
// 	last_pid = 0;
// 	prev_read_end = -1;
// 	env->pipeline = pipeline;
// 	while (pipeline)
// 	{
// 		if (pipeline->next)
// 		{
// 			if (pipe(pipe_fd) == -1)
// 			{
// 				perror("pipe");
// 				return (1);
// 			}
// 		}
// 		else
// 		{
// 			pipe_fd[READ_END] = -1;
// 			pipe_fd[WRITE_END] = -1;
// 		}
// 		last_pid = execute_one_pipeline_cmd(env, pipeline, prev_read_end,
// 				pipe_fd);
// 		if (last_pid == -1)
// 		{
// 			if (pipeline->next)
// 			{
// 				close(pipe_fd[READ_END]);
// 				close(pipe_fd[WRITE_END]);
// 			}
// 			return (1);
// 		}
// 		if (pipeline->next)
// 			prep_for_next_cmd(env, &prev_read_end, pipe_fd);
// 		else if (prev_read_end != -1)
// 			close(prev_read_end);
// 		pipeline = pipeline->next;
// 	}
// 	last_cmd_status = wait_for_children(env, last_pid, pipeline_count);
// 	printf("\n");
// 	return (last_cmd_status);
// }

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

int	execute_pipeline(t_env *env, t_ast_node *node)
{
	t_list	*pipeline;
	int		status;

	pipeline = build_cmd_list(env, node);
	if (!pipeline)
		return (1);
	status = execute_pipeline_cmds(env, pipeline);
	if (env->pipeline != NULL)
		free_pipeline_list(&env->pipeline);
	return (status);
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
		ft_printf("minishell: %s: Is not an executable binary\n",
			node->args[0]);
		free_ast(&env->root);
		free_env(env);
		free_envp(envp);
		exit(127);
	}
	if (execve(exec_path, node->args, envp) == -1)
	{
		ft_printf("minishell: %s: Is not an executable binary\n", exec_path);
		free_ast(&env->root);
		free(exec_path);
		free_envp(envp);
		free_env(env);
		exit(127);
	}
}

int	check_process_child_exit(t_env *env, int status)
{
	int	signal;

	(void)env;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		/*
			if (signal == SIGQUIT)
				write_s("Quit: 3", STDERR_FILENO, sh);
			if (signal == SIGQUIT || signal == SIGINT)
			{
				if (!new_line || (new_line && *new_line == false))
					write_s("\n", STDERR_FILENO, sh);
				if (new_line && *new_line == false)
					*new_line = true;
			}
		*/
		return (128 + signal);
	}
	else
		return (EXIT_FAILURE);
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
		fprintf(stderr, "[PID]execute_external_command(): %d\n", pid);
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
	// if (env->pipeline != NULL)
	// {
	// 	free_pipeline_list(&env->pipeline);
	// 	free_ast(&env->root);
	// 	free_env(env);
	// }
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
	fprintf(stderr, "[PID]execute_group(): %d\n", pid);
	if (pid == 0)
	{
		setup_child_signals(env, CHILD_SIG_CUSTOM);
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
	if (node->type == NODE_CMD)
	{
		execute_command_expansion(env, node);
		status = execute_command(env, node, exec_type);
	}
	else if (node->type == NODE_PIPE)
		status = execute_pipeline(env, node);
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
		status = execute_redirections(env, node);
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
	return (status);
}

int	execute_ast(t_env *env, t_ast_node *root, t_execute_type exec_type)
{
	return (execute(env, root, exec_type));
}
