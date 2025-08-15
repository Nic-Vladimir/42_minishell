/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:33:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 16:02:47 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
	TOK_AND,
	TOK_OR,
	TOK_GROUP_OPEN,
	TOK_GROUP_CLOSE,
	TOK_EOF,
	TOK_SGQ_BLOCK,
	TOK_DBQ_BLOCK,
	TOK_INVALID
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_tokenizer_data
{
	t_token			*tokens;
	t_token			*tail;
}					t_tokenizer_data;

void				free_tok_data(t_tokenizer_data *tok_data);
void				clear_tok_data(t_tokenizer_data *tok_data);
#endif
