/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:09:45 by mgavorni          #+#    #+#             */
/*   Updated: 2025/03/23 01:53:26 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_sig_action	m_sigint_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Ctrl-C Handler",
		.type = REAL_SIGNAL,
		.sig_handler = sig_handler,
		.sig_alt_handler = NULL,
		.virtual = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = SA_RESTART,
		.signum = SIGINT});
}

t_sig_action	m_sigquit_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Ctrl-\\ Handler",
		.type = REAL_SIGNAL,
		.sig_handler = SIG_IGN,
		.sig_alt_handler = NULL,
		.virtual = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = 0,
		.signum = SIGQUIT});
}

t_sig_action	m_sigchild_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Child exit Handler",
		.type = REAL_SIGNAL,
		.sig_handler = NULL,
		.sig_alt_handler = sig_alt_handler,
		.virtual = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = SA_SIGINFO | SA_RESTART,
		.signum = SIGCHLD});
}


void	setup_sig_handler(int id)
{
	sigset_t		sig_mask;
	size_t			i;
	t_sig_action	signals[SIGNAL_COUNT];

	rl_catch_signals = 0;
	sigemptyset(&sig_mask);
	signals[0] = m_sigint_handler(sig_mask);
	signals[1] = m_sigquit_handler(sig_mask);
	signals[2] = m_sigchild_handler(sig_mask);
	signals[3] = m_virtual_handler(sig_mask);
	i = 0;
	while (i < SIGNAL_COUNT)
	{
		if (signals[i].type == REAL_SIGNAL && register_sig(&signals[i]) == -1)
		{
			printf("Registration of : %s FAILED\n", signals[i].descript);
		}
		else if (signals[i].type == FAKE_SIGNAL && signals[i].id == id)
		{
			signals[i].virtual();
			break ;
		}
		i++;
	}
}
