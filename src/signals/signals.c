/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/signals.c
/*   Created: 2025/03/24 13:03:36 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/23 22:57:16 by mgavornik        ###   ########.fr       */
=======
/*   Created: 2025/03/27 11:13:28 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 02:37:09 by mgavorni         ###   ########.fr       */
>>>>>>> origin/mvp:src/signals/signals.c
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

volatile sig_atomic_t sig = 0;

// void	sig_handler(int sig)
// {
// 	(void)sig;
// 	rl_catch_signals = 0;
// 	g_glob_sig.sig = 1;
// 	while (g_glob_sig.sig)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 		g_glob_sig.sig = 0;
// 	}
// }

void set_signal_mode(int sig, t_sig_mode mode, t_sigenv *env)
{
<<<<<<< HEAD:src/signals.c
	struct sigaction sa;
=======
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
>>>>>>> origin/mvp:src/signals/signals.c

	sigemptyset(&sa.sa_mask);
	if (mode == MINI_MODE)
	{
		if(sig == SIGINT)
		{
			sa.sa_handler = mini_sigint_handler;
			sa.sa_flags = SA_RESTART;
			sigaction(sig, &sa, NULL);
		}
		else if(sig == SIGQUIT)
		{
			sa.sa_handler = SIG_IGN;
			sa.sa_flags = 0;
			sigaction(sig, &sa, NULL);
		}
		//g_glob_sig.current_mode = mode;

	}
	if (mode == NORMAL_MODE)
	{
		sigaction(SIGINT, &env->def->sigint, NULL);
		sigaction(SIGQUIT, &env->def->sigquit, NULL);
		//g_glob_sig.current_mode = mode;

	}
	if (mode == CD)
	{
		sigemptyset(&sa.sa_mask);

		sa.sa_handler = cd_handler;
		sa.sa_flags = SA_RESTART;
		sigaction(sig, &sa, NULL);
		//g_glob_sig.current_mode = mode;

	}

	 env->current_mode = mode;
	
}
void sig_malinit(t_sigenv **sigenv)
{
    *sigenv = (t_sigenv *)malloc(sizeof(t_sigenv));
    if (!*sigenv)
        return;
    (*sigenv)->def = (t_sig_def *)malloc(sizeof(t_sig_def));
    if (!(*sigenv)->def)
    {
        free(*sigenv);
        *sigenv = NULL;
        return;
    }
    (*sigenv)->current_mode = NORMAL_MODE;
    (*sigenv)->env = NULL;    
    sigaction(SIGINT, NULL, &(*sigenv)->def->sigint);
    sigaction(SIGQUIT, NULL, &(*sigenv)->def->sigquit);    
    set_all_signals(MINI_MODE, *sigenv);
}
<<<<<<< HEAD:src/signals.c

void set_all_signals(t_sig_mode mode, t_sigenv *sigenv)
{
	sigenv->current_mode = mode;
	set_signal_mode(SIGINT, mode, sigenv);
	set_signal_mode(SIGQUIT, mode, sigenv);
}

// t_sigenv init_signal_handlers(void)
// {
// 	t_sigenv defaults = NULL;

// 	sigaction(SIGINT, NULL, &defaults.sigint);
// 	sigaction(SIGQUIT, NULL, &defaults.sigquit);
// 	set_all_signals(MINI_MODE, &defaults);
// 	return (defaults);
// }



=======
>>>>>>> origin/mvp:src/signals/signals.c
