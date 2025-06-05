/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:39:37 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/05 16:34:40 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/pedo.h"
#include "../inc/minishell.h"

static void setup_child_signals(int behavior);

int execute_in_child(t_child_data *child_data)
{
    pid_t pid;
    int status;
    int result;
        
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return (-1);
    }
    if (pid == 0)
    {
        setup_child_signals(child_data->signal_behavior);
        result = child_data->func(child_data->data);
        if (child_data->cleanup)
            child_data->cleanup(child_data->data);    
        exit(result);
    }
    else
    {
        signal(SIGINT, SIG_IGN);
        waitpid(pid, &status, 0);
        // set_all_signals(MINI_MODE, child_data->sigenv);
        write(STDOUT_FILENO, "\n", 1);
        if (WIFSIGNALED(status))
            return (WEXITSTATUS(status));
        else
            return (1);
    }
}

static int heredoc_child_func(void *data)
{
    t_heredoc_data *hd = (t_heredoc_data *)data;
    return process_heredoc_input(hd->env, hd->delimiter, hd->write_fd);
}

void herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter)
{
    hd->env = env;
    hd->delimiter = delimiter;
}
void child_linker(t_child_data *child, t_heredoc_data *data ,int (*func)(void *data))
{
    child->data = data;
    child->func = func;
    child->cleanup = NULL;
    child->signal_behavior = CHILD_SIG_CUSTOM;
    child->sigenv = data->env->sigenv;
}
void init_structs(t_heredoc_data *hd, t_child_data *child)
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
    result = execute_in_child(&child_data);
    close(pipe_fds[1]);
    if (result != 0)
    {
        close(pipe_fds[0]);
        return (-1);
    }
    *write_fd = pipe_fds[0];
    return (0);
}

// Helper function for signal setup
static void setup_child_signals(int behavior)
{
    switch (behavior)
    {
        case CHILD_SIG_DEFAULT:
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            break;
        case CHILD_SIG_IGNORE:
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);
            break;
        case CHILD_SIG_CUSTOM:
            signal(SIGINT, tst_handler);
            signal(SIGQUIT, SIG_IGN);
            // Custom handlers would be set up here
            break;
    }
}