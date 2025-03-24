/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:03:36 by mgavorni          #+#    #+#             */
/*   Updated: 2025/03/24 13:03:42 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

t_sig_data		g_glob_sig = {0};

void	sig_handler(int sig)
{
	(void)sig;
	rl_catch_signals = 0;
	g_glob_sig.sig = 1;
	while (g_glob_sig.sig)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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
	else
	{
		printf("signal:[%d] received but no exited child\n", sig);
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


