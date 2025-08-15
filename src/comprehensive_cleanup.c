/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comprehensive_cleanup.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:42:27 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 15:49:31 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/env.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

void	do_ast(t_env **env)
{
	free_ast(&((*env)->root));
	(*env)->root = NULL;
}

void	do_tokens(t_env **env)
{
	free_tokens((*env)->tokenizer);
	free((*env)->tokenizer);
	(*env)->tokenizer = NULL;
}

void	do_pipeline(t_env **env)
{
	free_pipeline_list(&((*env)->pipeline));
	(*env)->pipeline = NULL;
}

void	do_input(t_env **env)
{
	free((*env)->input);
	(*env)->input = NULL;
}

void	comprehensive_cleanup(t_env **env)
{
	if (!env || !*env)
		return ;
	if ((*env)->root)
		do_ast(env);
	if ((*env)->tokenizer)
		do_tokens(env);
	if ((*env)->pipeline)
		do_pipeline(env);
	if ((*env)->input)
		do_input(env);
	if ((*env)->vars)
	{
		free_hashmap((*env)->vars);
		(*env)->vars = NULL;
	}
	if ((*env)->sigenv)
	{
		if ((*env)->sigenv->env && (*env)->sigenv->env == *env)
			(*env)->sigenv->env = NULL;
		free_sig((*env)->sigenv);
		(*env)->sigenv = NULL;
	}
	free(*env);
	*env = NULL;
	clean_rl();
}
