/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:24:49 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 15:32:00 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/pedo.h"

void	heredoc_child_cleanup(t_env *env)
{
	if (env)
	{
		if (env->root)
			free_ast(&env->root);
		if (env->tokenizer)
			free_tokens(env->tokenizer);
		if (env->pipeline)
			free_pipeline_list(&env->pipeline);
		free_env(env);
	}
}

void	herdoc_linker(t_heredoc_data *hd, t_env *env, char *delimiter)
{
	hd->env = env;
	hd->delimiter = delimiter;
}
