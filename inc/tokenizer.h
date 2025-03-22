/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:33:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/10 08:46:33 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

//TODO: Learn about multiple header inclusions

#include "arena.h"
typedef enum e_token_type {
    TOK_CMD,
    TOK_PIPE,
    TOK_REDIR_IN,
    TOK_REDIR_OUT,
    TOK_REDIR_APPEND,
    TOK_HEREDOC,
    TOK_AND,
    TOK_OR,
    TOK_GROUP_OPEN,
    TOK_GROUP_CLOSE,
    TOK_WORD,
    TOK_EOF,
} t_token_type;

typedef struct s_token {
	t_token_type type;
	char *value;
	struct s_token *next;
} t_token;

typedef struct s_tokenizer_data {
	t_arena *arena;
	t_token *tokens;
	t_token *tail;
} t_tokenizer_data;

#endif
