/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:42:02 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/17 09:05:07 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PEDO_H
#define PEDO_H

#define CHILD_SIG_DEFAULT 1
#define CHILD_SIG_CUSTOM 2
#define _POSIX_C_SOURCE 200809L

#include "../inc/minishell.h"
#include <asm-generic/signal-defs.h>
#include <bits/sigaction.h>
#include <signal.h>

typedef struct s_child_data
{
    void *data;                    
    int (*func)(void *data);       
    void (*cleanup)(void *data);
    int signal_behavior;
    void *sigenv;
} t_child_data;

typedef struct s_heredoc_data
{
    t_env *env;
    char *delimiter;
    int write_fd;
} t_heredoc_data;


#endif