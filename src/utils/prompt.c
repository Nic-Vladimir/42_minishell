/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:30:29 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:36:32 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_transient_prompt(char *command)
{
	ft_printf("\033[A\r\033[K");
	ft_printf("\033[A\r\033[K");
	ft_printf("\033[38;5;46m❯\033[0m %s\n", command);
}

static char	*format_prompt(const char *prefix, const char *cwd,
		const char *suffix)
{
	char	*prompt;
	size_t	prompt_len;
	size_t	offset;

	prompt_len = ft_strlen(prefix) + ft_strlen(cwd) + ft_strlen(suffix) + 1;
	prompt = (char *)malloc(prompt_len);
	if (!prompt)
		return (ft_strdup("minishell> "));
	offset = 0;
	offset += ft_strlcpy(prompt + offset, prefix, prompt_len - offset);
	offset += ft_strlcpy(prompt + offset, cwd, prompt_len - offset);
	offset += ft_strlcpy(prompt + offset, suffix, prompt_len - offset);
	return (prompt);
}

char	*get_prompt(t_env *env)
{
	char	*cwd;
	char	*prefix;
	char	*suffix;
	char	*prompt;

	cwd = get_env_value(env, "PWD");
	if (env->last_exit_code == 0)
	{
		prefix = PROMPT_PREFIX_OK;
	}
	else
		prefix = PROMPT_PREFIX_KO;
	suffix = PROMPT_SUFFIX;
	prompt = format_prompt(prefix, cwd, suffix);
	return (prompt);
}
