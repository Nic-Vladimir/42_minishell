/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_hand.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:09:02 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/24 16:55:18 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SIG_HAND_H
#define SIG_HAND_H

#include <sys/signal.h>
#include <sys/wait.h>
#include <signal.h>
#include "../inc/minishell.h"

typedef struct s_env t_env;

typedef struct s_sig_def
{
    struct sigaction sigint;
    struct sigaction sigquit;
    
} t_sig_def;

typedef enum e_sig_mode
{
    MINI_MODE,
    NORMAL_MODE,
    CD,
} t_sig_mode;

typedef struct s_sigenv
{
    t_sig_mode current_mode;
    t_env *env;
    t_sig_def *def;
    
}t_sigenv;


extern volatile sig_atomic_t sig;  

// extern t_sig_data g_glob_sig;

void sig_malinit(t_sigenv **sigenv);

void set_signal_mode(int sig, t_sig_mode mode, t_sigenv *env);
void set_all_signals(t_sig_mode mode, t_sigenv *sigenv);
t_sig_def init_signal_handlers(void);
void cd_handler(int sig, t_env *env);
void mini_sigint_handler(int sig);


#endif
