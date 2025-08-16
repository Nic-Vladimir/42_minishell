/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:02:13 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/16 15:36:44 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ast.h"
#include "../../inc/minishell.h"
#include "../../inc/tokenizer.h"

t_ast_node	*parse_group(t_tokenizer_data *tok_data);

t_ast_node	*parse_pipeline(t_tokenizer_data *tok_data)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_group(tok_data);
	while (tok_data->tokens && tok_data->tokens->type == TOK_PIPE)
	{
		tok_data->tokens = tok_data->tokens->next;
		right = parse_group(tok_data);
		left = ast_node_insert(left, NODE_PIPE, NULL);
		left->right = right;
	}
	return (left);
}

t_ast_node	*parse_logical_operators(t_tokenizer_data *tok_data)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_node_type	type;

	left = parse_pipeline(tok_data);
	while (tok_data->tokens && (tok_data->tokens->type == TOK_AND
			|| tok_data->tokens->type == TOK_OR))
	{
		if ((*tok_data->tokens).type == TOK_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		tok_data->tokens = tok_data->tokens->next;
		right = parse_pipeline(tok_data);
		left = ast_node_insert(left, type, NULL);
		left->right = right;
	}
	return (left);
}

t_ast_node	*parse_group(t_tokenizer_data *tok_data)
{
	t_ast_node	*subtree;
	t_ast_node	*group_node;

	if (tok_data->tokens && tok_data->tokens->type == TOK_GROUP_OPEN)
	{
		tok_data->tokens = tok_data->tokens->next;
		subtree = parse_logical_operators(tok_data);
		if (!tok_data->tokens || tok_data->tokens->type != TOK_GROUP_CLOSE)
		{
			ft_printf("Syntax error: expected closing ')'\n");
			return (NULL);
		}
		tok_data->tokens = tok_data->tokens->next;
		group_node = ast_new_node(NODE_GROUP, NULL);
		if (!group_node)
			return (NULL);
		group_node->group = subtree;
		return (group_node);
	}
	return (parse_simple_command(tok_data));
}

t_ast_node	*parse(t_tokenizer_data *tok_data)
{
	t_tokenizer_data	*tok_data_cpy;

	tok_data_cpy = tok_data;
	return (parse_logical_operators(tok_data_cpy));
}
