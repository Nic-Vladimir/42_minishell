/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:00:26 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/29 21:04:08 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	validate_special_cases(t_token *current)
{
	if (current->type == TOK_HEREDOC)
	{
		if (!current->next || current->next->type != TOK_WORD)
			return (false);
	}
	return (true);
}

bool	is_redirection_token(t_token_type type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_REDIR_APPEND || type == TOK_HEREDOC);
}

bool	is_operator_token(t_token_type type)
{
	return (type == TOK_PIPE || type == TOK_AND || type == TOK_OR
		|| type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_REDIR_APPEND || type == TOK_HEREDOC
		|| type == TOK_GROUP_OPEN || type == TOK_GROUP_CLOSE);
}
