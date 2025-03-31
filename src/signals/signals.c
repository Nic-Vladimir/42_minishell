/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:13:28 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 02:37:09 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_sig_data		g_glob_sig = {0};

void	sig_handler(int sig)
{
	//ssize_t status;
	(void)sig;
	rl_catch_signals = 0;
	g_glob_sig.sig = 1;
	//status = 0;
	while (g_glob_sig.sig)
	{
		// status = write(STDOUT_FILENO, "\n", 1);
		// if(status == -1)
		// 	g_glob_sig.sig = -1;
        char *prompt = rl_prompt;              // Get full prompt string
        char *first_line = prompt;             // Start of first line
        char *newline_pos = strchr(prompt, '\n');  // Find newline
        char *current_input = rl_line_buffer;  // Get interrupted input

        write(STDOUT_FILENO, "\n", 1);         // Move past partial input
        if (newline_pos)
        {
            size_t len = newline_pos - first_line;  // Length of first line
            write(STDOUT_FILENO, first_line, len);  // Print first line
            write(STDOUT_FILENO, "\n", 1);      // Newline after first line
        }
        else
            write(STDOUT_FILENO, "\n", 1);      // Fallback if no newline
        //write(STDOUT_FILENO, "\n", 1);         // Blank line
        rl_replace_line("", 0);                // Clear input buffer
        rl_on_new_line();                      // Sync after initial newline
        rl_on_new_line();                      // Sync after first line
        rl_on_new_line();                      // Sync after blank line
        rl_redisplay();                        // Redisplay full prompt
        if (current_input && *current_input)
            printf("%s", current_input);       // Print interrupted input on new prompt
        g_glob_sig.sig = 0;
	}
}

pid_t	child_ret(siginfo_t *info, int *status)
{
	pid_t	pid;

	if (!info)
		return (waitpid(-1, status, WNOHANG));
	pid = info->si_pid;
	if (waitpid(pid, status, WNOHANG) <= 0)
		return (-1);
	return (pid);
}

void	sig_alt_handler(int sig, siginfo_t *info, void *context)
{
	int		status;
	pid_t	pid;

	(void)context;
	pid = child_ret(info, &status);
	if (pid > 0)
	{
		if (WIFEXITED(status))
			printf("signal:[%d] child PID:[%d] exit code %d\n",
				sig, pid, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			printf("signal:[%d] child PID:[%d] kill signal %d\n",
				sig, pid, WTERMSIG(status));
	}
}

int	register_sig(const t_sig_action *config)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = config->sig_flag;
	sa.sa_mask = config->sig_mask;
	if (config->sig_flag & SA_SIGINFO)
		sa.sa_sigaction = config->sig_alt_handler;
	else
		sa.sa_handler = config->sig_handler;
	if (sigaction(config->signum, &sa, NULL) == -1)
	{
		if (config->descript)
		{
			perror(config->descript);
		}
		else
		{
			perror("register_sig : sigaction");
		}
		return (-1);
	}
	return (0);
}
