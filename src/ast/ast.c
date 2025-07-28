/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:02:13 by vnicoles          #+#    #+#             */
/*   Updated: 2025/07/28 11:15:00 by vnicoles         ###   ########.fr       */
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
			printf("Syntax error: expected closing ')'\n");
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

/*
// DEBUGGING FUNCTIONS ---
// Function to get string representation of node types
const char	*node_type_to_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	else if (type == NODE_PIPE)
		return ("PIPE");
	else if (type == NODE_REDIR_IN)
		return ("REDIR_IN");
	else if (type == NODE_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == NODE_REDIR_APPEND)
		return ("APPEND");
	else if (type == NODE_HEREDOC)
		return ("HEREDOC");
	else if (type == NODE_AND)
		return ("AND");
	else if (type == NODE_OR)
		return ("OR");
	else if (type == NODE_GROUP)
		return ("GROUP");
	else
		return ("UNKNOWN");
}

// Helper function to print indentation
void	print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
	{
		printf("  ");
	}
}

// Recursive function to display the AST
void	display_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	printf("Node: %s\n", node_type_to_str(node->type));
	// Display args for command nodes
	if (node->type == NODE_CMD && node->args)
	{
		print_indent(depth + 1);
		printf("Args: ");
		for (int i = 0; node->args[i]; i++)
		{
			printf("\"%s\" ", node->args[i]);
		}
		printf("\n");
	}
	// Display filename for redirection nodes
	if ((node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
			|| node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC)
		&& node->args)
	{
		print_indent(depth + 1);
		printf("File: \"%s\"\n", node->args[0]);
	}
	// Recursively display children
	if (node->left)
	{
		print_indent(depth);
		printf("Left:\n");
		display_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		print_indent(depth);
		printf("Right:\n");
		display_ast(node->right, depth + 1);
	}
}

// Main function to debug an AST
void	debug_ast(t_ast_node *root)
{
	printf("=== AST Debug Output ===\n");
	display_ast(root, 0);
	printf("=======================\n");
}
*/
