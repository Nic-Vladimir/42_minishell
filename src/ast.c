/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:02:13 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 23:12:59 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/ast.h"
#include "../inc/tokenizer.h"
#include <stdio.h>

t_ast_node *parse_group(t_tokenizer_data *tok_data);

t_ast_node *ast_set_node_arg_types(t_ast_node *node, char **args)
{
	int			i;
	int			j;

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
                return NULL;
            }
            while (j < i)
                node->arg_types[j++] = TOK_WORD;
        }
        else
            node->arg_types = NULL;
    }
	return node;
}

t_ast_node *ast_new_node(t_node_type type, char **args)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;
    node->type = type;
    node->args = args;
    node->arg_types = NULL;
	node->left = NULL;
	node->right = NULL;
	node = ast_set_node_arg_types(node, args);
	return node;
}

t_ast_node *ast_node_insert(t_ast_node *root, t_node_type type, char **args) {
	t_ast_node	*new_root;

	if (!root)
		return ast_new_node(type, args);
	if (type == NODE_PIPE || type == NODE_AND || type == NODE_OR) {
		new_root = ast_new_node(type, NULL);
		new_root->left = root;
		new_root->right = NULL;

        if (!args)
            new_root->right = ast_new_node(NODE_CMD, args);
		return new_root;
	}
	root->right = ast_node_insert(root->right, type, args);
	return root;
}

void	ast_execute(t_ast_node *node) {
	if (!node)
		return;
	ast_execute(node->left);
	printf("Executing: %s\n", node->args ? node->args[0] : "[OPERATOR]");
	ast_execute(node->right);
}

t_ast_node *parse_redirection(t_tokenizer_data *tok_data, t_ast_node *cmd) {
    t_token     *redir;
    t_ast_node  *redir_node;
    char        **args;

    redir = tok_data->tokens;
    tok_data->tokens = (*tok_data->tokens).next;
    if (!tok_data->tokens || tok_data->tokens->type != TOK_WORD) {
        printf("Syntax error: expected filename after redirection\n");
        return NULL;
    }
    switch (redir->type) {
        case TOK_REDIR_IN:
            redir_node = ast_new_node(NODE_REDIR_IN, NULL);
            break;
        case TOK_REDIR_APPEND:
            redir_node = ast_new_node(NODE_REDIR_APPEND, NULL);
            break;
        case TOK_REDIR_OUT:
            redir_node = ast_new_node(NODE_REDIR_OUT, NULL);
            break;
        case TOK_HEREDOC:
            redir_node = ast_new_node(NODE_HEREDOC, NULL);
            break;
        default:
            printf("Syntax error: unnexpected redirection type\n");
            return NULL;
    }
    redir_node->right = cmd;
    if ((*tok_data->tokens).value) {
        args = (char **)malloc(2 * sizeof(char *));
        if (!args)
        {
            free(redir_node);
            return NULL;
        }

        args[0] = ft_strdup(tok_data->tokens->value);
        args[1] = NULL;
        redir_node->args = args;
    } else
        redir_node->args = NULL;
    tok_data->tokens = tok_data->tokens->next;
    return redir_node;
}
void    init_t_ast(t_ast_node *node)
{
    node->arg_types = NULL;
    node->args = NULL;
    node->left = NULL;
    node->right = NULL;
    node->type = NODE_CMD;
}

t_ast_node *init_cmd_node(char **args, int arg_count)
{
	t_ast_node	*node;
    int         j;

    j = 0;
	node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;
    init_t_ast(node);
    node->args = args;
    if (arg_count > 0)
        {
            node->arg_types = malloc(sizeof(t_token_type) * arg_count);
            if (!node->arg_types)
            {
                free(node);
                free(args);
                return NULL;
            }
            while (j < arg_count)
                node->arg_types[j++] = TOK_WORD;
        }
	return node;
}

t_ast_node *parse_simple_command(t_tokenizer_data *tok_data) {
    t_ast_node  *cmd;
    char        **args;
    int         arg_count;
    t_token     *temp;
    int         i;
    t_token     *current;

    arg_count = 0;
    temp = tok_data->tokens;
    while (temp && (temp->type == TOK_WORD || temp->type == TOK_SGQ_BLOCK || temp->type == TOK_DBQ_BLOCK)) {
        arg_count++;
        temp = temp->next;
    }
    args = malloc(sizeof(char *) * (arg_count + 1));
    if (!args)
        return NULL;
    printf("Args: [%d]\n", arg_count);
    //cmd = ast_new_node(NODE_CMD, args);
    cmd = init_cmd_node(args, arg_count);
	if (!cmd)
	{
		free(args);
		return NULL;
	}
    i = 0;
    current = tok_data->tokens;
    while (current && (current->type == TOK_WORD ||
								current->type == TOK_SGQ_BLOCK ||
								current->type == TOK_DBQ_BLOCK)) {
		args[i] = ft_strdup(current->value);
		cmd->arg_types[i++] = current->type;
        current = current->next;
    }
    args[i] = NULL;
    tok_data->tokens = current;
    while (tok_data->tokens && (tok_data->tokens->type == TOK_REDIR_IN || tok_data->tokens->type == TOK_REDIR_OUT ||
                                tok_data->tokens->type == TOK_REDIR_APPEND || tok_data->tokens->type == TOK_HEREDOC)) {
        cmd = parse_redirection(tok_data, cmd);
    }
    return cmd;
}

t_ast_node *parse_pipeline(t_tokenizer_data *tok_data) {
    t_ast_node  *left;
    t_ast_node  *right;

    left = parse_group(tok_data);
    while (tok_data->tokens && tok_data->tokens->type == TOK_PIPE) {
        tok_data->tokens = tok_data->tokens->next;
        right = parse_group(tok_data);
        left = ast_node_insert(left, NODE_PIPE, NULL);
        left->right = right;
    }
    return left;
}

t_ast_node *parse_logical_operators(t_tokenizer_data *tok_data) {
	t_ast_node	*left;
	t_ast_node	*right;
	t_node_type	type;

	left = parse_pipeline(tok_data);
	while(tok_data->tokens && (tok_data->tokens->type == TOK_AND || tok_data->tokens->type == TOK_OR)) {
		if ((*tok_data->tokens).type == TOK_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		tok_data->tokens = tok_data->tokens->next;
		right = parse_pipeline(tok_data);
		left = ast_node_insert(left, type, NULL);
		left->right = right;
	}
	return left;
}

t_ast_node *parse_group(t_tokenizer_data *tok_data) {
    t_ast_node	*group;
	t_ast_node	*group_node;
	
	if (tok_data->tokens && tok_data->tokens->type == TOK_GROUP_OPEN) {
		tok_data->tokens = tok_data->tokens->next;
		group = parse_logical_operators(tok_data);
		if (!tok_data->tokens || tok_data->tokens->type != TOK_GROUP_CLOSE) {
			printf("Syntax error: expected closing ')'\n");
			return NULL;
		}
		tok_data->tokens = tok_data->tokens->next;
		group_node = ast_new_node(NODE_GROUP, NULL);
		if (!group_node)
			return NULL;
		group_node->left = group;
		return group_node;
	}
	return parse_simple_command(tok_data);
}

t_ast_node *parse(t_tokenizer_data *tok_data) {
    t_tokenizer_data    *tok_data_cpy;

    tok_data_cpy = tok_data;
	return parse_logical_operators(tok_data_cpy);
}

// DEBUGGING FUNCTIONS ---
// Function to get string representation of node types
const char *node_type_to_str(t_node_type type) {
    switch (type) {
        case NODE_CMD: return "CMD";
        case NODE_PIPE: return "PIPE";
        case NODE_REDIR_IN: return "REDIR_IN";
        case NODE_REDIR_OUT: return "REDIR_OUT";
        case NODE_REDIR_APPEND: return "APPEND";
        case NODE_HEREDOC: return "HEREDOC";
        case NODE_AND: return "AND";
        case NODE_OR: return "OR";
        case NODE_GROUP: return "GROUP";
        default: return "UNKNOWN";
    }
}

// Helper function to print indentation
void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

// Recursive function to display the AST
void display_ast(t_ast_node *node, int depth) {
    if (!node)
        return;
    
    print_indent(depth);
    printf("Node: %s\n", node_type_to_str(node->type));
    
    // Display args for command nodes
    if (node->type == NODE_CMD && node->args) {
        print_indent(depth + 1);
        printf("Args: ");
        for (int i = 0; node->args[i]; i++) {
            printf("\"%s\" ", node->args[i]);
        }
        printf("\n");
    }
    
    // Display filename for redirection nodes
    if ((node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT || 
         node->type == NODE_REDIR_APPEND || node->type == NODE_HEREDOC) && node->args) {
        print_indent(depth + 1);
        printf("File: \"%s\"\n", node->args[0]);
    }
    
    // Recursively display children
    if (node->left) {
        print_indent(depth);
        printf("Left:\n");
        display_ast(node->left, depth + 1);
    }
    
    if (node->right) {
        print_indent(depth);
        printf("Right:\n");
        display_ast(node->right, depth + 1);
    }
}

// Main function to debug an AST
void debug_ast(t_ast_node *root) {
    printf("=== AST Debug Output ===\n");
    display_ast(root, 0);
    printf("=======================\n");
}
