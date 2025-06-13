/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:13:28 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/13 13:54:32 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include <signal.h>
#include <bits/sigaction.h>
#include <asm-generic/signal-defs.h>

volatile sig_atomic_t	sig = 0;

static void set_mini_mode(int sig)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	if (sig == SIGINT)
	{
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = mini_sigint_handler;
	}
	else
	{
		sa.sa_flags = 0;
		sa.sa_handler = SIG_IGN;
	}
	sigaction(sig, &sa, NULL);
}

static void set_normal_mode(int sig, t_sigenv *env)
{
	if (sig == SIGINT)
		sigaction(SIGINT, &env->def->sigint, NULL);
	else if (sig == SIGQUIT)
		sigaction(SIGQUIT, &env->def->sigquit, NULL);
}

static void set_cd_mode(int sig, t_sigenv *env)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = NULL;
	sa.sa_flags = SA_RESTART;
	sigaction(sig, &sa, NULL);
	env->status = 1;
}

void set_signal_mode(int sig, t_sig_mode mode, t_sigenv *env)
{
	if (mode == MINI_MODE)
		set_mini_mode(sig);
	else if (mode == NORMAL_MODE)
		set_normal_mode(sig, env);
	else if (mode == CD)
		set_cd_mode(sig, env);

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
