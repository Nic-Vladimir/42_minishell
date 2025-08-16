/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:59:00 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:37:05 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ast.h"
#include "../../inc/minishell.h"

static t_ast_node	*create_redirection_node(t_token_type redir_type)
{
	t_ast_node	*redir_node;

	if (redir_type == TOK_REDIR_IN)
		redir_node = ast_new_node(NODE_REDIR_IN, NULL);
	else if (redir_type == TOK_REDIR_APPEND)
		redir_node = ast_new_node(NODE_REDIR_APPEND, NULL);
	else if (redir_type == TOK_REDIR_OUT)
		redir_node = ast_new_node(NODE_REDIR_OUT, NULL);
	else if (redir_type == TOK_HEREDOC)
		redir_node = ast_new_node(NODE_HEREDOC, NULL);
	else
	{
		ft_printf("Syntax error: unnexpected redirection type\n");
		return (NULL);
	}
	return (redir_node);
}

static int	set_redirection_args(t_ast_node *redir_node,
		t_token *filename_token)
{
	char	**args;

	if (filename_token->value)
	{
		args = (char **)malloc(2 * sizeof(char *));
		if (!args)
			return (0);
		ft_memset(args, 0, 2 * sizeof(char *));
		args[0] = ft_strdup(filename_token->value);
		args[1] = NULL;
		redir_node->args = args;
	}
	else
		redir_node->args = NULL;
	return (1);
}

t_ast_node	*parse_redirection(t_tokenizer_data *tok_data, t_ast_node *cmd)
{
	t_token		*redir;
	t_ast_node	*redir_node;

	redir = tok_data->tokens;
	tok_data->tokens = (*tok_data->tokens).next;
	if (!tok_data->tokens || tok_data->tokens->type != TOK_WORD)
	{
		ft_printf("Syntax error: expected filename after redirection\n");
		return (NULL);
	}
	redir_node = create_redirection_node(redir->type);
	if (!redir_node)
		return (NULL);
	redir_node->right = cmd;
	if (!set_redirection_args(redir_node, tok_data->tokens))
	{
		free(redir_node);
		return (NULL);
	}
	tok_data->tokens = tok_data->tokens->next;
	return (redir_node);
}
