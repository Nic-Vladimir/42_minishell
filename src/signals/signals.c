/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:13:28 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:38:43 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <asm-generic/signal-defs.h>
#include <bits/sigaction.h>
#include <signal.h>

volatile sig_atomic_t	g_sig = 0;

void	set_mini_mode(sig_atomic_t g_sig)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	if (g_sig == SIGINT)
	{
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = mini_sigint_handler;
	}
	else
	{
		sa.sa_flags = 0;
		sa.sa_handler = SIG_IGN;
	}
	sigaction(g_sig, &sa, NULL);
}

static void	set_normal_mode(sig_atomic_t g_sig, t_sigenv *env)
{
	if (g_sig == SIGINT)
		sigaction(SIGINT, &env->def->sigint, NULL);
	else if (g_sig == SIGQUIT)
		sigaction(SIGQUIT, &env->def->sigquit, NULL);
}

static void	set_cd_mode(sig_atomic_t g_sig, t_sigenv *env)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = NULL;
	sa.sa_flags = SA_RESTART;
	sigaction(g_sig, &sa, NULL);
	env->status = 1;
}

void	set_signal_mode(sig_atomic_t g_sig, t_sig_mode mode, t_sigenv *env)
{
	if (mode == MINI_MODE)
		set_mini_mode(g_sig);
	else if (mode == NORMAL_MODE)
		set_normal_mode(g_sig, env);
	else if (mode == CD)
		set_cd_mode(g_sig, env);
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
