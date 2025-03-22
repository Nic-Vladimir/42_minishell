/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:35:58 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/10 08:46:44 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef enum e_node_type {
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_GROUP,
} t_node_type;

typedef struct s_ast_node {
	t_node_type type;
	char **args;
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;


#endif
