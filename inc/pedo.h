/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:42:02 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/05 15:24:27 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PEDO_H
#define PEDO_H

#define CHILD_SIG_DEFAULT 0
#define CHILD_SIG_IGNORE 1
#define CHILD_SIG_CUSTOM 2

#include "../inc/minishell.h"
typedef struct s_child_data
{
    void *data;                    //data for func
    int (*func)(void *data);       //func execute in child
    void (*cleanup)(void *data);   //cleanup function
    int signal_behavior;       //handle signals
    void *sigenv;
} t_child_data;

typedef struct s_heredoc_data
{
    t_env *env;
    char *delimiter;
    int write_fd;
} t_heredoc_data;


int execute_in_child(t_child_data *child_data);
int collect_heredoc(t_env *env, char *delimiter, int *write_fd);
void init_structs(t_heredoc_data *hd, t_child_data *child);
void child_linker(t_child_data *child, t_heredoc_data *data ,int (*func)(void *data));
void herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter);

#endif