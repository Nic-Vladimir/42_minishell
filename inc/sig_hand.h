/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_hand.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:09:02 by mgavorni          #+#    #+#             */
/*   Updated: 2025/06/22 22:17:24 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_HAND_H
# define SIG_HAND_H

# include <signal.h>
# include <sys/signal.h>
# include <sys/wait.h>

typedef struct s_env			t_env;

typedef struct s_sig_def
{
	struct sigaction			sigint;
	struct sigaction			sigquit;
}								t_sig_def;

typedef enum e_sig_mode
{
	MINI_MODE = 2,
	NORMAL_MODE,
	CD,
}								t_sig_mode;

typedef struct s_sigenv
{
	t_sig_mode					current_mode;
	t_env						*env;
	t_sig_def					*def;
	int							status;
}								t_sigenv;

extern volatile sig_atomic_t	g_sig;

void							sig_malinit(t_sigenv **sigenv);
void							set_signal_mode(sig_atomic_t g_sig,
									t_sig_mode mode, t_sigenv *env);
void							set_all_signals(t_sig_mode mode,
									t_sigenv *sigenv);
void							cd_handler(sig_atomic_t g_sig, t_env *env);
void							mini_sigint_handler(sig_atomic_t g_sig);
#endif
