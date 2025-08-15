/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:43:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 15:36:11 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	validate_pipe_and_logical_rules(t_token *current, t_token *prev)
{
	if (current->type == TOK_PIPE)
	{
		if (!prev || !current->next || current->next->type == TOK_EOF)
			return (false);
		if (prev->type == TOK_AND || prev->type == TOK_OR
			|| prev->type == TOK_PIPE)
			return (false);
	}
	else if (current->type == TOK_AND || current->type == TOK_OR)
	{
		if (!prev || !current->next || current->next->type == TOK_EOF)
			return (false);
		if (prev->type == TOK_AND || prev->type == TOK_OR
			|| prev->type == TOK_PIPE)
			return (false);
	}
	return (true);
}

static bool	validate_parentheses_rules(t_token *current, t_token *prev,
		int *paren_count)
{
	if (current->type == TOK_GROUP_OPEN)
	{
		(*paren_count)++;
		if (current->next && is_operator_token(current->next->type)
			&& current->next->type != TOK_GROUP_CLOSE)
			return (false);
	}
	else if (current->type == TOK_GROUP_CLOSE)
	{
		(*paren_count)--;
		if (*paren_count < 0)
			return (false);
		if (prev && is_operator_token(prev->type)
			&& prev->type != TOK_GROUP_OPEN)
			return (false);
	}
	return (true);
}

static bool	validate_operator_rules(t_token *current, t_token *prev)
{
	if (prev && is_operator_token(prev->type))
	{
		if (prev->type == TOK_GROUP_OPEN && current->type == TOK_GROUP_CLOSE)
			return (false);
	}
	if (!prev && current->type != TOK_GROUP_OPEN)
		return (false);
	if ((!current->next || current->next->type == TOK_EOF)
		&& current->type != TOK_GROUP_CLOSE)
		return (false);
	return (true);
}

static bool	validate_redirection_rules(t_token *current, t_token *prev)
{
	if (!current->next || current->next->type == TOK_EOF)
		return (false);
	if (!(current->next->type == TOK_WORD
			|| current->next->type == TOK_SGQ_BLOCK
			|| current->next->type == TOK_DBQ_BLOCK))
		return (false);
	if (prev && is_redirection_token(prev->type))
	{
		if (current->type == TOK_REDIR_IN && (prev->type == TOK_REDIR_OUT
				|| prev->type == TOK_REDIR_APPEND))
			return (false);
	}
	return (true);
}

bool	validate_single_token(t_token *current, t_token *prev, int *paren_count)
{
	if (is_operator_token(current->type))
	{
		if (!validate_operator_rules(current, prev))
			return (false);
	}
	if (is_redirection_token(current->type))
	{
		if (!validate_redirection_rules(current, prev))
			return (false);
	}
	if (current->type == TOK_PIPE || current->type == TOK_AND
		|| current->type == TOK_OR)
	{
		if (!validate_pipe_and_logical_rules(current, prev))
			return (false);
	}
	if (!validate_parentheses_rules(current, prev, paren_count))
		return (false);
	if (!validate_special_cases(current))
		return (false);
	return (true);
}
