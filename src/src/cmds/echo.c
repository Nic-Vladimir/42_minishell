/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:04:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/27 04:50:35 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int execute_echo(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    int is_flag;
    int i;

    (void)env;
    (void)in_fd;
    i = 1;
    is_flag = 0;
    if(node->args[i] && ft_strcmp(node->args[i], "-n") == 0)
    {
        is_flag = 1;
        i++;
    }
    while(node->args[i])
    {
        write(out_fd, node->args[i], ft_strlen(node->args[i]));
        if (node->args[i + 1])
        {
            write(out_fd, " ", 1);
        }
        i++;
    }
    if(!is_flag)
        write(out_fd, "\n", 1);
    return 0;
}
