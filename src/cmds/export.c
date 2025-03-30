/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:17:49 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 21:41:52 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdlib.h>

void swap(char **a, char **b)
{
    char    *temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

int partition(char **envp, int low, int high)
{
    int     i;
    int     j;
    char    *pivot;

    pivot = envp[high];
    i = low - 1;
    j = low;
    while (j <= high - 1)
    {
        if (ft_strcmp(envp[j], pivot) <= 0)
        {
            i++;
            swap(&envp[i], &envp[j]);
        }
        j++;
    }
    swap(&envp[i+1], &envp[high]);
    return i + 1;
}

void sort_envp(char **envp, int low, int high)
{
    int index;

    if (low < high)
    {
        index = partition(envp, low, high);
        sort_envp(envp, low, index-1);
        sort_envp(envp, index+1, high);
    }
}

static int print_sorted_envp(t_env *env, int out_fd)
{
    int     i;
    char    **envp;
    ssize_t write_status;

    i = 0;
    envp = get_envp_from_hashmap(env);
    while (envp[i])
        i++;
    sort_envp(envp, 0, i - 1);
    i = 0;
    while (envp[i])
    {
        write_status = write(out_fd, envp[i], ft_strlen(envp[i]));
       if (write_status == -1 || write(out_fd, "\n", 1) == -1)
       {
        free_envp(envp);
        return (1);
       }
        i++;
    }
    free_envp(envp);
    return 0;
}

int	execute_export(t_env *env, t_ast_node *node, int in_fd, int out_fd)
{
	char	*key;
	char	*value;
	char	*temp;
	int		status;
	int		i;
	char	*equal;

    (void)in_fd;
	i = 1;
    if (!node->args[i])
    {
        return print_sorted_envp(env, out_fd);
    }
	while (node->args[i])
	{
		temp = node->args[i++];
		equal = ft_strchr(temp, '=');
		if (equal)
		{
			key = ft_substr(temp, 0, equal - temp);
			value = ft_substr(temp, equal - temp + 1, ft_strlen(equal) + 1);
		}
		else
		{
			key = ft_strdup(temp);
			value = ft_strdup("");
		}
		printf("key= [%s]\n", key);
		printf("value= [%s]\n", value);

		status = hashmap_insert(env, key, value);
		printf("inserted: [%s]\n", get_env_value(env, key));
		free(key);
		free(value);
	}
	return status;
}

void free_envp(char **envp)
{
    int i;
    if(!envp)
        return;
    i = 0;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

