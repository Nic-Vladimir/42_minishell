/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:02:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/27 03:21:11 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int execute_exit(t_env *env)
{
    free_tokens(env->tokenizer);
    free_ast(env->root);
    free_env(env);
    exit(0);
}
