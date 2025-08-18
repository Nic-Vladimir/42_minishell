/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:16:04 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/18 11:28:19 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	custom_prompt_handler(void)
{
	char	*prompt;
	char	*newline_pos;
	char	*current_input;
	size_t	len;

	prompt = rl_prompt;
	newline_pos = ft_strchr(prompt, '\n');
	current_input = rl_line_buffer;
	write(STDOUT_FILENO, "\n", 1);
	if (newline_pos)
	{
		len = newline_pos - prompt;
		write(STDOUT_FILENO, prompt, len);
		write(STDOUT_FILENO, "\n", 1);
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	if (ft_strcmp("heredoc> ", prompt) != 0)
	{
		rl_redisplay();
	}
	if (current_input && *current_input)
		ft_printf("%s", current_input);
	g_sig = 130;
}

void	mini_sigint_handler(sig_atomic_t g_sig)
{
	static int	first_interrupt = 1;

	rl_catch_signals = 0;
	if (g_sig)
	{
		if (first_interrupt)
		{
			first_interrupt = 0;
		}
		else
		{
			write(STDOUT_FILENO, "^C", 2);
		}
		custom_prompt_handler();
	}
}

void	cd_handler(sig_atomic_t g_sig, t_env *env)
{
	ft_printf("exit\n");
	execute_exit(env, NULL, g_sig);
}

void	set_all_signals(t_sig_mode mode, t_sigenv *sigenv)
{
	sigenv->current_mode = mode;
	set_signal_mode(SIGINT, mode, sigenv);
	set_signal_mode(SIGPIPE, mode, sigenv);
	set_signal_mode(SIGQUIT, mode, sigenv);
}
