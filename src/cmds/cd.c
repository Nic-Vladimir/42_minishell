/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:03:32 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/26 21:22:51 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int cd_change_dir(char *path, char **old_pwd) 
{
    char *prev_dir = NULL;
    char *curent_dir = NULL;
   
    prev_dir = getcwd(NULL, 0);
    if (!prev_dir) 
    {
        perror(PURPLE"getcwd failed before chdir"RESET);
        return(0);
    }
    if (chdir(path) == -1) 
    {
        perror(PURPLE"cd failed"RESET);
        free(prev_dir);
        return(0);
    }
    free(*old_pwd);
    *old_pwd = prev_dir;
    curent_dir = getcwd(NULL, 0);
    if (curent_dir) 
    {
        printf(PURPLE"Directory changed to: %s\n"RESET, curent_dir);
        free(curent_dir);
    } 
    else 
    {
        perror(PURPLE"getcwd failed after chdir"RESET);
    }
    return(1);
}

int execute_cd(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    char    *home;
    char    *path;
    static char *old_pwd = NULL;

    (void)in_fd;
    (void)out_fd;
    path = node->args[1];
    home = get_env_value(env, "HOME");
    if(path == NULL || ft_strcmp(path, "~") == 0)
        path = home;
    else if(ft_strcmp(path, "-") == 0)
    {
        if (old_pwd)
        {
            printf(YELLOW"%s\n"RESET, old_pwd);
            path = old_pwd;
        }
        else
        {
            printf(PURPLE"cd: prev PWD not set\n"RESET);
            return 1;
        }
    }
    if(!cd_change_dir(path, &old_pwd))
        return 1;
    return 0;
 
}
