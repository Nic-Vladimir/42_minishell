/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:05:11 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/26 20:55:24 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int execute_pwd(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    char    *cwd;
    ssize_t len;

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
        write(out_fd, cwd, len);
        write(out_fd, "\n", 1);
        return 0;
    }
    else
    {
        perror(YELLOW"getcwd err"RESET);
        return 1;
    }
}
