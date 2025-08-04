/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 03:02:59 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/04 09:04:05 by mgavornik        ###   ########.fr       */
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
	// Debug printing BEFORE freeing structures
	if (env)
	{
		free_everything(&env);
	}
	else
	{
		fprintf(stderr, "[DEBUG] execute_exit called with NULL env pointer\n");
		fprintf(stderr, "[DEBUG] g_sig (exit code): %d\n", (int)g_sig);
	}

	clean_rl();
	exit(g_sig);
}

void	child_exit(int exit_code)
{
	fprintf(stderr, "[DEBUG] Child process %d exiting with code %d\n", getpid(), exit_code);
	exit(exit_code);
}
