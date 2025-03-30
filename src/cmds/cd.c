/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:03:32 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 22:53:34 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char *check_path(t_env *env, char *input_path)
{
    char *path;
    char *home;

    path = input_path;
    home = get_env_value(env, "HOME");
    if (!path || ft_strcmp(path, "~") == 0)
    {
        if (!home)
            return NULL;
        path = home;
    }
    else if (ft_strcmp(path, "-") == 0)
    {
        path = get_env_value(env, "OLDPWD");
        if (!path)
            return NULL;
    }
    fancy_write(1, path, PURPLE);
    return path;
}

static int change_dir(char *path, char **old_pwd)
{
    if(!path)
        return(1);

    *old_pwd = getcwd(NULL, 0);
    if (!*old_pwd)
    {
        perror("cd: getcwd failed");
        return 1;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        free(*old_pwd);
        *old_pwd = NULL;
        return 1;
    }
    return 0;
}

static int update_env(t_env *env, char *old_pwd)
{
    char *new_pwd;
    int result;

    result = 0;
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("cd: getcwd failed");
        free(old_pwd);
        return 1;
    }
    if (old_pwd && hashmap_insert(env, "OLDPWD", old_pwd) != 0)
        fancy_write(1,"cd: failed to update OLDPWD", RED);
    if (hashmap_insert(env, "PWD", new_pwd) != 0)
        result = 1; // Keep going, but note the error
    free(old_pwd);
    free(new_pwd);
    return result;
}

int execute_cd(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
    char *path;
    char *old_pwd;
    int result;

    (void)in_fd;
    (void)out_fd;
    old_pwd = NULL;
    path = check_path(env, node->args[1]);
    if (!path)
        return 1;
    result = change_dir(path, &old_pwd);
    if (result != 0)
        return 1;
    result = update_env(env, old_pwd);
    if (result != 0)
        return 1;
    return 0;
}
