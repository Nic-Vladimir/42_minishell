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
#include <readline/history.h>
#include <readline/readline.h>

void	ft_free_split(char **res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
}

void	clean_rl(void)
{
	rl_cleanup_after_signal();
	clear_history();
	rl_deprep_terminal();
}

static int	is_numeric(const char *str)
{
	if (!str || !*str || *str == '-')
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	execute_exit(t_env *env, t_ast_node *node)
{
	long long	temp_code;
	int			exit_code;

	/*
	i = 0;
	while (node->args[i])
	{
		printf("arg[%d]: %s\n", i, node->args[i]);
		i++;
	}
	*/
	if (!node->args[1])
		exit_code = env->last_exit_code;
	printf("node->args[1]: '%s'\n", node->args[1]);
	if (node->args[1] && node->args[2])
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	else if (is_numeric(node->args[1]))
	{
		temp_code = ft_atoi(node->args[1]);
		exit_code = (int)(temp_code % 256);
		if (exit_code < 0)
			exit_code += 256;
	}
	else
		exit_code = 2;
	free_ast(env->root);
	free_env(env);
	clean_rl();
	exit(exit_code);
}
