/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:03:36 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/23 16:56:25 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/


#include "../inc/minishell.h"

t_sig_data g_glob_sig = {0};

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

void set_signal_mode(int sig, t_sig_mode mode, t_sig_def *def)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	if (mode == MINI_MODE && sig == SIGINT)
	{
		sigemptyset(&sa.sa_mask);

		sa.sa_handler = mini_sigint_handler;
		sa.sa_flags = SA_RESTART;
		sigaction(sig, &sa, NULL);
	}
	else if (mode == MINI_MODE && sig == SIGQUIT)
	{
		sigemptyset(&sa.sa_mask);

		sa.sa_handler = SIG_IGN;
		sa.sa_flags = 0;
		sigaction(sig, &sa, NULL);
	}
	else if (mode == NORMAL_MODE)
	{
		sigemptyset(&sa.sa_mask);

		sigaction(SIGINT, &def->sigint, NULL);
		sigaction(SIGQUIT, &def->sigquit, NULL);
	}
	else if (mode == CD)
	{
		sigemptyset(&sa.sa_mask);

		sa.sa_handler = cd_handler;
		sa.sa_flags = SA_RESTART;
		sigaction(sig, &sa, NULL);
	}

	g_glob_sig.current_mode = mode;
	
}

void set_all_signals(t_sig_mode mode, t_sig_def *def)
{
	
	set_signal_mode(SIGINT, mode, def);
	set_signal_mode(SIGQUIT, mode, def);
}

t_sig_def init_signal_handlers(void)
{
	t_sig_def defaults;

	sigaction(SIGINT, NULL, &defaults.sigint);
	sigaction(SIGQUIT, NULL, &defaults.sigquit);
	set_all_signals(MINI_MODE, &defaults);
	return (defaults);
}



