/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:13:28 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/04 01:16:36 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

volatile sig_atomic_t	sig = 0;

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

void	set_signal_mode(int sig, t_sig_mode mode, t_sigenv *env)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	if (mode == MINI_MODE)
	{
		if (sig == SIGINT)
		{
			sa.sa_handler = mini_sigint_handler;
			sa.sa_flags = SA_RESTART;
			sigaction(sig, &sa, NULL);
		}
		else if (sig == SIGQUIT)
		{
			sa.sa_handler = SIG_IGN;
			sa.sa_flags = 0;
			sigaction(sig, &sa, NULL);
		}
		// g_glob_sig.current_mode = mode;
	}
	if (mode == NORMAL_MODE)
	{
		sigaction(SIGINT, &env->def->sigint, NULL);
		sigaction(SIGQUIT, &env->def->sigquit, NULL);
		// g_glob_sig.current_mode = mode;
	}
	if (mode == CD)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_handler = NULL;
		sa.sa_flags = SA_RESTART;
		sigaction(sig, &sa, NULL);
		env->status = 1;		// g_glob_sig.current_mode = mode;
	}
	env->current_mode = mode;
}

void	sig_malinit(t_sigenv **sigenv)
{
	*sigenv = (t_sigenv *)malloc(sizeof(t_sigenv));
	if (!*sigenv)
	{
		return ;
	}
	ft_memset(*sigenv, 0, sizeof(t_sigenv));
	(*sigenv)->def = (t_sig_def *)malloc(sizeof(t_sig_def));
	if (!(*sigenv)->def)
	{
		free(*sigenv);
		*sigenv = NULL;
		return ;
	}
	ft_memset((*sigenv)->def, 0, sizeof(t_sig_def));
	(*sigenv)->current_mode = NORMAL_MODE;
	(*sigenv)->env = NULL;
	sigaction(SIGINT, NULL, &(*sigenv)->def->sigint);
	sigaction(SIGQUIT, NULL, &(*sigenv)->def->sigquit);
	set_all_signals(NORMAL_MODE, *sigenv);
}

void	set_all_signals(t_sig_mode mode, t_sigenv *sigenv)
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
