/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:02:13 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/10 08:56:10 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/ast.h"
#include "../inc/tokenizer.h"
#include <stdio.h>

t_ast_node *parse_group(t_tokenizer_data *tok_data);

t_ast_node *ast_new_node(t_arena *arena, t_node_type type, char **args) {
	t_ast_node	*node;

	node = (t_ast_node *)arena_malloc(arena, sizeof(t_ast_node));
    if (!node)
        return NULL;
    node->type = type;
    node->args = args;
	node->left = NULL;
	node->right = NULL;
    return node;
}

t_ast_node *ast_node_insert(t_arena *arena, t_ast_node *root, t_node_type type, char **args) {
	t_ast_node	*new_root;

	if (!root)
		return ast_new_node(arena, type, args);
	if (type == NODE_PIPE || type == NODE_AND || type == NODE_OR) {
		new_root = ast_new_node(arena, type, NULL);
		new_root->left = root;
		new_root->right = NULL;

        if (!args)
            new_root->right = ast_new_node(arena, NODE_CMD, args);
		return new_root;
	}
	root->right = ast_node_insert(arena, root->right, type, args);
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
            redir_node = ast_new_node(tok_data->arena, NODE_REDIR_IN, NULL);
            break;
        case TOK_REDIR_APPEND:
            redir_node = ast_new_node(tok_data->arena, NODE_REDIR_APPEND, NULL);
            break;
        case TOK_REDIR_OUT:
            redir_node = ast_new_node(tok_data->arena, NODE_REDIR_OUT, NULL);
            break;
        case TOK_HEREDOC:
            redir_node = ast_new_node(tok_data->arena, NODE_HEREDOC, NULL);
            break;
        default:
            printf("Syntax error: unnexpected redirection type\n");
            return NULL;
    }
    redir_node->right = cmd;
    if ((*tok_data->tokens).value) {
        args = (char **)arena_malloc(tok_data->arena, 2 * sizeof(char *));
        if (args == NULL)
            return NULL;
        args[0] = tok_data->tokens->value;
        args[1] = NULL;
        redir_node->args = args;
    } else
        redir_node->args = NULL;
    tok_data->tokens = tok_data->tokens->next;
    return redir_node;
}

t_ast_node *parse_simple_command(t_tokenizer_data *tok_data) {
    t_ast_node  *cmd;
    char        **args;
    int         arg_count;
    t_token     *temp;
    int         i;

    i = 0;
    arg_count = 0;
    temp = tok_data->tokens;
    while (temp && temp->type == TOK_WORD) {
        arg_count++;
        temp = temp->next;
    }
    args = arena_malloc(tok_data->arena, sizeof(char *) * (arg_count + 1));
    if (!args)
        return NULL;
    while (tok_data->tokens && tok_data->tokens->type == TOK_WORD) {
        args[i++] = tok_data->tokens->value;
        tok_data->tokens = tok_data->tokens->next;
    }
    args[i] = NULL;
    cmd = ast_new_node(tok_data->arena, NODE_CMD, args);
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
        left = ast_node_insert(tok_data->arena, left, NODE_PIPE, NULL);
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
		left = ast_node_insert(tok_data->arena, left, type, NULL);
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
		group_node = ast_new_node(tok_data->arena, NODE_GROUP, NULL);
		if (!group_node)
			return NULL;
		group_node->left = group;
		return group_node;
	}
	return parse_simple_command(tok_data);
}

t_ast_node *parse(t_tokenizer_data *tok_data) {
	return parse_logical_operators(tok_data);
}


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
