/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:39:37 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/13 20:35:23 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#define _POSIX_C_SOURCE 200809L // Ensure POSIX features like siginfo_t are enabled
#include "../inc/minishell.h"
#include "../inc/pedo.h"
#include <signal.h>
#include <bits/sigaction.h>
#include <asm-generic/signal-defs.h>

static void	setup_child_signals(t_env *env, int behavior);
static t_env *local_env = NULL;
void set_signal_env(t_env *env)
{
	local_env = env;
}

static void	child_process(t_env *env, t_child_data *child_data, int in_fd,
		int out_fd)
{
	int	result;
    
	setup_child_fds(in_fd, out_fd);
	setup_child_signals(env, child_data->signal_behavior);
	result = child_data->func(child_data->data);
	if (child_data->cleanup)
		child_data->cleanup(child_data->data);
	exit(result);
}

static int parent_process(pid_t pid, t_env *env, int in_fd, int out_fd)
{
	int status;
	(void)env;
	
	signal(SIGINT, SIG_IGN);  
	waitpid(pid, &status, 0);
	set_all_signals(MINI_MODE, env->sigenv); 
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (128 + WTERMSIG(status));
}


int	execute_in_child(t_env *env, t_child_data *child_data, int in_fd,
		int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
    {
		child_process(env, child_data, in_fd, out_fd);
    }
	return (parent_process(pid, env, in_fd, out_fd));
}

static int heredoc_child_func(void *data)
{
    t_heredoc_data *hd;
    
    hd = (t_heredoc_data *)data;
    return (process_heredoc_input(hd->env, hd->delimiter, hd->write_fd));
}


void	herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter)
{
	hd->env = env;
	hd->delimiter = delimiter;
}
int	execute_exit_wrapper(void *data)
{
	return (execute_exit(data));
}
void	child_linker(t_child_data *child, t_heredoc_data *data,
		int (*func)(void *data))
{
	child->data = data;
	child->func = func;
	child->cleanup = NULL;
	child->signal_behavior = CHILD_SIG_CUSTOM;
	child->sigenv = data->env->sigenv;
}
void	init_structs(t_heredoc_data *hd, t_child_data *child)
{
	ft_memset(hd, 0, sizeof(t_heredoc_data));
	ft_memset(child, 0, sizeof(t_child_data));
}
int collect_heredoc(t_env *env, char *delimiter, int *write_fd)
{
    int pipe_fds[2];
    t_heredoc_data hd_data;
    t_child_data child_data;
    int result;

    if (pipe(pipe_fds) == -1)
    {
        perror("pipe failed");
        return (-1);
    }
    init_structs(&hd_data, &child_data);
    herdoc_linker(&hd_data, env, delimiter);
    hd_data.write_fd = pipe_fds[1];
    child_linker(&child_data, &hd_data, heredoc_child_func);
    result = execute_in_child(env, &child_data, STDIN_FILENO, STDOUT_FILENO);
    close(pipe_fds[1]);
    if (result != 0)
    {
        close(pipe_fds[0]);
        return (-1);
    }
    *write_fd = pipe_fds[0];
    return (0);
}

static void	custom_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_env	*env;

	(void)ucontext;
    env = (t_env *)info->si_value.sival_ptr; 
    if(sig == SIGINT)
    {
	    mini_sigint_handler(sig);
		execute_exit(env);
    }
  // (void)env;
}
void	setup_child_signals(t_env *env, int behavior)
{
	struct sigaction	sa;
	union sigval 	value;
	if (behavior == CHILD_SIG_CUSTOM)
	{
		memset(&sa, 0, sizeof(sa));
		memset(&value, 0, sizeof(value));
		value.sival_ptr = env;
		sa.sa_sigaction = custom_handler;
		sa.sa_flags = SA_RESTART;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGINT, &sa, NULL) == -1)
			perror("sigaction");
	}
    
}
