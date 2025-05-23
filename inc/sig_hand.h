/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_hand.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 10:31:03 by mgavornik         #+#    #+#             */
/*   Updated: 2025/05/23 16:07:00 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SIG_HAND_H
#define SIG_HAND_H

#include <sys/signal.h>
#include <sys/wait.h>
#include <signal.h>
#include "../inc/minishell.h"

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

typedef struct s_sig_data 
{
    volatile sig_atomic_t sig;
    t_sig_mode current_mode;
    t_env *env;
    t_sig_def def;
    
} t_sig_data;

extern t_sig_data g_glob_sig;

void set_signal_mode(int sig, t_sig_mode mode, t_sig_def *def);
void set_all_signals(t_sig_mode mode, t_sig_def *def);
t_sig_def init_signal_handlers(void);
void cd_handler(int sig);
void mini_sigint_handler(int sig);


#endif
