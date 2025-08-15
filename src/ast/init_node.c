/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:11:16 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:17:30 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ast.h"
#include "../../inc/minishell.h"

static void	init_t_ast(t_ast_node *node)
{
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = NODE_CMD;
}

t_ast_node	*init_cmd_node(char **args, int arg_count)
{
	t_ast_node	*node;
	int			j;

	node = NULL;
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
		ft_memset(node->arg_types, 0, sizeof(t_token_type) * arg_count);
		while (j < arg_count)
			node->arg_types[j++] = TOK_WORD;
	}
	return (node);
}

static t_ast_node	*ast_set_node_arg_types(t_ast_node *node, char **args)
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
			ft_memset(node->arg_types, 0, sizeof(t_token_type) * i);
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
		return (new_root);
	}
	root->right = ast_node_insert(root->right, type, args);
	return (root);
}
