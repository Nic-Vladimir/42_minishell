/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:02:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/18 11:19:17 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <termios.h>

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
	rl_clear_history();
}

void	reset_terminal_for_readline(void)
{
	struct termios	term;

	if (tcgetattr(0, &term) == 0)
	{
		term.c_lflag |= (ICANON | ECHO);
		tcsetattr(0, TCSANOW, &term);
	}
	rl_on_new_line();
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
}

int	execute_exit(t_env *env, t_ast_node *node, sig_atomic_t g_sig)
{
	if (node && node->args[1])
		g_sig = ft_atoi(node->args[1]);
	else
		g_sig = 0;
	if (!env)
	{
		exit(g_sig);
	}
	if (env->shell_pid == ft_getpid())
	{
		comprehensive_cleanup(&env);
		clean_rl();
		exit(g_sig);
	}
	else
	{
		comprehensive_cleanup(&env);
		exit(g_sig);
	}
}

int	execute_cleaning(t_env *env)
{
	if (env)
	{
		if (env->root)
			free_ast(&(env->root));
		if (env->tokenizer)
			free_tokens(env->tokenizer);
		if (env->pipeline)
			free_pipeline_list(&env->pipeline);
	}
	return (0);
}
