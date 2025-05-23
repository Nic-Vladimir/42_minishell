/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:04:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 01:31:45 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int execute_echo(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    int is_flag;
    int i;
    ssize_t status;

    (void)env;
    (void)in_fd;
    status = 0;
    i = 1;
    is_flag = 0;
    if(node->args[i] && ft_strcmp(node->args[i], "-n") == 0)
    {
        is_flag = 1;
        i++;
    }
    while(node->args[i])
    {
       status = write(out_fd, node->args[i], ft_strlen(node->args[i]));
       if (status == -1)
            return(1);
        if (node->args[i + 1])
        {
            if(write(out_fd, " ", 1) == -1)
                return(1);
        }
        i++;
    }
    if(!is_flag)
    {
        if (write(out_fd, "\n", 1) == -1)
            return (1);
    }
     
    return 0;
}
