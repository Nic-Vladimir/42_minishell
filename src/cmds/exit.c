/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:02:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/13 17:21:43 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	rl_cleanup_after_signal();
	clear_history();
	rl_deprep_terminal();
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

int		execute_exit(t_env *env, sig_atomic_t g_sig)
{
	execute_cleaning(env);
	clean_rl();
	free_env(env);
	exit(g_sig);
}

int execute_cleaning(t_env *env)
{
	if(env)
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
