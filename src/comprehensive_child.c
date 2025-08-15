/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comprehensive_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:53:29 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 15:53:59 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/env.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

void	child_comprehensive_cleanup(t_env *env)
{
	if (!env)
		return ;
	if (env->root)
	{
		free_ast(&(env->root));
		env->root = NULL;
	}
	if (env->pipeline)
	{
		free_pipeline_list(&(env->pipeline));
		env->pipeline = NULL;
	}
	if (env->tokenizer)
	{
		clear_tok_data(env->tokenizer);
	}
	if (env->input)
	{
		free(env->input);
		env->input = NULL;
	}
}

void	force_cleanup_all_memory(t_env **env)
{
	if (!env || !*env)
		return ;
	comprehensive_cleanup(env);
	rl_clear_history();
}

void	free_everything(t_env **env)
{
	if (!env)
	{
		return ;
	}
	force_cleanup_all_memory(env);
}
