/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pedo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:39:37 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/17 09:10:45 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/minishell.h"
#include "../inc/pedo.h"

void	child_linker(t_child_data *child, t_heredoc_data *data,
		int (*func)(void *data))
{
	child->data = data;
	child->func = func;
	child->cleanup = NULL;
	child->signal_behavior = CHILD_SIG_CUSTOM;
	child->sigenv = data->env->sigenv;
}

void	init_structs(t_heredoc_data *hd, t_child_data *child)
{
	ft_memset(hd, 0, sizeof(t_heredoc_data));
	ft_memset(child, 0, sizeof(t_child_data));
}

static void	custom_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_env	*env;

	(void)ucontext;
	env = (t_env *)info->si_value.sival_ptr;
	if (sig == SIGINT)
	{
		mini_sigint_handler(sig);
		execute_exit(env);
	}
}
void	setup_child_signals(t_env *env, int behavior)
{
	struct sigaction	sa;
	union sigval		value;

	if (behavior == CHILD_SIG_CUSTOM)
	{
		memset(&sa, 0, sizeof(sa));
		memset(&value, 0, sizeof(value));
		value.sival_ptr = env;
		sa.sa_sigaction = custom_handler;
		sa.sa_flags = SA_RESTART;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGINT, &sa, NULL) == -1)
			perror("sigaction");
	}
}
