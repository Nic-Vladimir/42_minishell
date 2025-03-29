/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:16:04 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/27 11:30:12 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_sig_action	m_sigint_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Ctrl-C Handler",
		.type = REAL_SIGNAL,
		.sig_handler = sig_handler,
		.sig_alt_handler = NULL,
		.virt = NULL,
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
		.virt = NULL,
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
		.virt = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = SA_SIGINFO | SA_RESTART,
		.signum = SIGCHLD});
}


void	init_sig(int id)
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
			signals[i].virt();
			break ;
		}
		i++;
	}
}/*
void sigquit_handler(int signum)
{
    (void)signum;
}

void sigint_handler(int signum)
{
    (void)signum;
    print_transient_prompt("");
}

void init_sig(void)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = sigquit_handler;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
*/
