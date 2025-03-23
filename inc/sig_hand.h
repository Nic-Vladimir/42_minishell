/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_hand.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:09:02 by mgavorni          #+#    #+#             */
/*   Updated: 2025/03/23 02:38:15 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_HAND_H
# define SIG_HAND_H

# define _POSIX_C_SOURCE 200809L

# include <sys/signal.h>
# include <sys/wait.h>
// # include <asm-generic/siginfo.h>
// # include <asm-generic/signal.h>

# define SIGNAL_COUNT 4
# define SIG_VIRTUAL_CTRL_D 1
# define SIG_REAL 0

// --- Handler function typedefs ---
typedef void	(*t_real_af)(int);
typedef void	(*t_real_af_plus)(int, siginfo_t *, void *);
typedef void	(*t_fake_af)(void);

// --- Signal type: real vs virtual ---
typedef enum e_signal_type_e
{
	REAL_SIGNAL,
	FAKE_SIGNAL
}							t_signal_type;

// --- Global signal state ---
typedef struct sig_data_s
{
	volatile sig_atomic_t	sig;
}							t_sig_data;

// --- Signal action configuration ---
typedef struct sig_action_s
{
	const char				*descript;       // short description
	t_signal_type			type;            // REAL_SIGNAL or FAKE_SIGNAL
	t_real_af				sig_handler;     // handler for classic signal
	t_real_af_plus			sig_alt_handler; // handler for SA_SIGINFO
	t_fake_af				virtual;         // fake handler function
	sigset_t				sig_mask;        // signal mask
	int						id;      		 // ID for virtual handler
	int						sig_flag;        // sa flags
	int						signum;          // signal number SIGINT
}							t_sig_action;

// --- Globals ---
extern t_sig_data			g_glob_sig;
extern t_sig_action			signals[SIGNAL_COUNT];

// --- Core signal system API ---
int							register_sig(const t_sig_action *config);
void						setup_sig_handler(int id);
void						setup_virt_handler(int id);

// --- General-purpose handlers ---
void						sig_handler(int sig);
void						sig_alt_handler(int sig, siginfo_t *info, void *context);
void						virtual(void);
pid_t						child_ret(siginfo_t *info, int *status);

// --- Signal configuration builders ---
t_sig_action				m_virtual_handler(sigset_t sig_mask);
t_sig_action				m_sigchild_handler(sigset_t sig_mask);
t_sig_action				m_sigquit_handler(sigset_t sig_mask);
t_sig_action				m_sigint_handler(sigset_t sig_mask);

#endif
