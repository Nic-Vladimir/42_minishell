/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:35:58 by vnicoles          #+#    #+#             */
/*   Updated: 2025/07/28 09:33:13 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenizer.h"

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_GROUP,
	NODE_SGQ,
	NODE_DBQ
}						t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	t_token_type		*arg_types;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*group;
}						t_ast_node;

#endif
