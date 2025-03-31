/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:02:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 02:22:07 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
void ft_free_split(char **res)
{
    int i = 0;
    while (res[i])
    {
        free(res[i]);
        i++;
    }
    free(res);
}
void clean_rl(void)
{
    rl_cleanup_after_signal();
    clear_history();
    rl_deprep_terminal();
}

int execute_exit(t_env *env)
{
    //free_tokens(env->tokenizer);
    free_ast(env->root);
    free_env(env);
    clean_rl();
    exit(0);
}
