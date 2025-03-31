/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:02:13 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 02:10:30 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <stdio.h>

t_ast_node	*parse_group(t_token **tokens);

t_ast_node	*ast_set_node_arg_types(t_ast_node *node, char **args)
{
	int	i;
	int	j;

	if (args)
	{
		i = 0;
		j = 0;
		while (args[i])
			i++;
		if (i > 0)
		{
			node->arg_types = malloc(sizeof(t_token_type) * i);
			if (!node->arg_types)
			{
				free(node);
				return (NULL);
			}
			while (j < i)
				node->arg_types[j++] = TOK_WORD;
		}
		else
			node->arg_types = NULL;
	}
	return (node);
}

t_ast_node	*ast_new_node(t_node_type type, char **args)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	init_t_ast(node);
	node->type = type;
	node->args = args;
	// node->arg_types = NULL;
	// node->left = NULL;
	// node->right = NULL;
	node = ast_set_node_arg_types(node, args);
	return (node);
}

t_ast_node	*ast_node_insert(t_ast_node *root, t_node_type type, char **args)
{
	t_ast_node	*new_root;

	if (!root)
		return (ast_new_node(type, args));
	if (type == NODE_PIPE || type == NODE_AND || type == NODE_OR)
	{
		new_root = ast_new_node(type, NULL);
		new_root->left = root;
		new_root->right = NULL;
		if (!args)
			new_root->right = ast_new_node(NODE_CMD, args);
		return (new_root);
	}
	root->right = ast_node_insert(root->right, type, args);
	return (root);
}

void	ast_execute(t_ast_node *node)
{
	if (!node)
		return ;
	ast_execute(node->left);
	printf("Executing: %s\n", node->args ? node->args[0] : "[OPERATOR]");
	ast_execute(node->right);
}

t_ast_node	*parse_redirection(t_token **tokens, t_ast_node *cmd)
{
	t_token		*redir;
	t_ast_node	*redir_node;
	char		**args;

	if (!tokens || !*tokens)
		return (cmd); // No redirection, return cmd as is
	redir = *tokens;
	*tokens = (*tokens)->next; // Advance past redirection token
	if (!*tokens || (*tokens)->type != TOK_WORD)
	{
		printf("Syntax error: expected filename after redirection\n");
		return (NULL); // Caller will free cmd if needed
	}
	switch (redir->type)
	{
	case TOK_REDIR_IN:
		redir_node = ast_new_node(NODE_REDIR_IN, NULL);
		break ;
	case TOK_REDIR_APPEND:
		redir_node = ast_new_node(NODE_REDIR_APPEND, NULL);
		break ;
	case TOK_REDIR_OUT:
		redir_node = ast_new_node(NODE_REDIR_OUT, NULL);
		break ;
	case TOK_HEREDOC:
		redir_node = ast_new_node(NODE_HEREDOC, NULL);
		break ;
	default:
		printf("Syntax error: unexpected redirection type\n");
		return (NULL);
	}
	if (!redir_node)
		return (NULL);       // Allocation failed, let caller handle cmd
	redir_node->right = cmd; // Attach command as right child
	if ((*tokens)->value)
	{
		args = malloc(2 * sizeof(char *));
		if (!args)
		{
			free_ast(redir_node); // Free node, cmd preserved
			return (NULL);
		}
		args[0] = ft_strdup((*tokens)->value);
		if (!args[0])
		{
			free(args);
			free_ast(redir_node);
			return (NULL);
		}
		args[1] = NULL;
		redir_node->args = args;
	}
	else
		redir_node->args = NULL;
	*tokens = (*tokens)->next; // Advance past filename
	return (redir_node);
}
void	init_t_ast(t_ast_node *node)
{
	node->arg_types = NULL;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->type = NODE_CMD;
}

t_ast_node	*init_cmd_node(char **args, int arg_count)
{
	t_ast_node	*node;
	int			j;

	j = 0;
	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	init_t_ast(node);
	node->args = args;
	if (arg_count > 0)
	{
		node->arg_types = malloc(sizeof(t_token_type) * arg_count);
		if (!node->arg_types)
		{
			free(node);
			free(args);
			return (NULL);
		}
		while (j < arg_count)
			node->arg_types[j++] = TOK_WORD;
	}
	return (node);
}

t_ast_node	*parse_simple_command(t_token **tokens)
{
	t_ast_node	*cmd;
	char		**args;
	int			arg_count;
	t_token		*temp;
	int			i;
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);
	arg_count = 0;
	temp = *tokens;
	while (temp && (temp->type == TOK_WORD || temp->type == TOK_SGQ_BLOCK
			|| temp->type == TOK_DBQ_BLOCK))
	{
		arg_count++;
		temp = temp->next;
	}
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	printf("Args: [%d]\n", arg_count);    // Debug, remove if not needed
	cmd = init_cmd_node(args, arg_count); // Assuming this allocs t_ast_node
	if (!cmd)
	{
		free(args);
		return (NULL);
	}
	i = 0;
	current = *tokens;
	while (current && (current->type == TOK_WORD
			|| current->type == TOK_SGQ_BLOCK
			|| current->type == TOK_DBQ_BLOCK))
	{
		args[i] = ft_strdup(current->value);
		if (!args[i]) // Check strdup failure
		{
			while (i > 0)
				free(args[--i]);
			free(args);
			free_ast(cmd);
			return (NULL);
		}
		cmd->arg_types[i++] = current->type;
		current = current->next;
	}
	args[i] = NULL;
	*tokens = current; // Advance tokens
	while (*tokens && ((*tokens)->type == TOK_REDIR_IN
			|| (*tokens)->type == TOK_REDIR_OUT
			|| (*tokens)->type == TOK_REDIR_APPEND
			|| (*tokens)->type == TOK_HEREDOC))
	{
		cmd = parse_redirection(tokens, cmd); // Adjust this call too
	}
	return (cmd);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipe_node;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_group(tokens);
	while (*tokens && (*tokens)->type == TOK_PIPE)
	{
		*tokens = (*tokens)->next; // Advance past '|'
		right = parse_group(tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		pipe_node = ast_new_node(NODE_PIPE, NULL); // Create pipe node
		if (!pipe_node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		pipe_node->left = left;   // Attach left subtree
		pipe_node->right = right; // Attach right subtree
		left = pipe_node;         // New root for next iteration
	}
	return (left);
}

t_ast_node	*parse_logical_operators(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_node_type	type;
	t_token		*current;
	t_ast_node	*op_node;

	left = parse_pipeline(tokens);
	current = *tokens;
	while (current && (current->type == TOK_AND || current->type == TOK_OR))
	{
		if (current->type == TOK_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		*tokens = current->next;
		right = parse_logical_operators(tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		op_node = ast_new_node(type, NULL);
		if (!op_node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		op_node->left = left;
		op_node->right = right;
		return (op_node);
	}
	return (left);
}

t_ast_node	*parse_group(t_token **tokens)
{
	t_ast_node	*group;
	t_ast_node	*group_node;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOK_GROUP_OPEN)
	{
		*tokens = (*tokens)->next; // Skip '('
		group = parse_logical_operators(tokens);
		if (!*tokens || (*tokens)->type != TOK_GROUP_CLOSE)
		{
			printf("Syntax error: expected closing ')'\n");
			free_ast(group);
			return (NULL);
		}
		*tokens = (*tokens)->next; // Skip ')'
		group_node = ast_new_node(NODE_GROUP, NULL);
		if (!group_node)
		{
			free_ast(group);
			return (NULL);
		}
		group_node->left = group;
		group_node->right = NULL; // Explicitly set
		return (group_node);
	}
	return (parse_simple_command(tokens)); // Adjust this too
}

t_ast_node	*parse(t_tokenizer_data *tok_data)
{
	t_tokenizer_data	*tok_data_cpy;

	tok_data_cpy = tok_data;
	return (parse_logical_operators(&tok_data_cpy->tokens));
}

// DEBUGGING FUNCTIONS ---
// Function to get string representation of node types
const char	*node_type_to_str(t_node_type type)
{
	switch (type)
	{
	case NODE_CMD:
		return ("CMD");
	case NODE_PIPE:
		return ("PIPE");
	case NODE_REDIR_IN:
		return ("REDIR_IN");
	case NODE_REDIR_OUT:
		return ("REDIR_OUT");
	case NODE_REDIR_APPEND:
		return ("APPEND");
	case NODE_HEREDOC:
		return ("HEREDOC");
	case NODE_AND:
		return ("AND");
	case NODE_OR:
		return ("OR");
	case NODE_GROUP:
		return ("GROUP");
	default:
		return ("UNKNOWN");
	}
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
