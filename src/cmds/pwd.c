/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:05:11 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 21:41:36 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int execute_pwd(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    char    *cwd;
    ssize_t len;
    ssize_t status;

    status = 0;
    (void)in_fd;
    cwd = get_env_value(env, "PWD");
    if (node->args[1])
    {
        ft_printf(YELLOW"pwd: too many arguments\n"RESET);
        return 1;
    }
    else if (*cwd)
    {
        len = ft_strlen(cwd);
        status = write(out_fd, cwd, len);
        if(status == -1 || write(out_fd, "\n", 1) == -1)
            return(1);
        return 0;
    }
    else
    {
        perror(YELLOW"getcwd err"RESET);
        return 1;
    }
}
