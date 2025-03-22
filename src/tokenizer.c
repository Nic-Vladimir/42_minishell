/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:40:49 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/18 20:14:44 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tokenizer.h"
#include "../inc/minishell.h"


static t_token	*new_token(t_tokenizer_data *tok_data, t_token_type type, char *value, size_t len) {
	t_token *token;

	token = arena_malloc(tok_data->arena, sizeof(t_token));
	if (!token)
		return NULL;
	token->type = type;
	token->value = arena_malloc(tok_data->arena, len + 1);
	if (!token->value)
		return NULL;
	memcpy(token->value, value, len);
	token->value[len] = '\0';
	token->next = NULL;
	return token;
}

static void	add_token(t_tokenizer_data *tok_data, t_token_type type, char *value, size_t len) {
	t_token *new;

	new = new_token(tok_data, type, value, len);
	if(!new)
		return;
	if (!tok_data->tokens) {
		tok_data->tokens = new;
		tok_data->tail = new;
	} else {
		tok_data->tail->next = new;
        tok_data->tail = new;
	}
}

static char *process_word_token(t_tokenizer_data *tok_data, char *input) {
    char *start;

	start = input;
    while (*input && !isspace(*input) && *input != '|' && *input != '<' && *input != '>' && *input != ')')
        input++;
    add_token(tok_data, TOK_WORD, start, input - start);
    return input;
}

t_token	*tokenize(t_tokenizer_data *tok_data, char *input) {
	while (*input) {
		if (*input == '|') {
			if (*(input + 1) == '|') {
				add_token(tok_data, TOK_OR, "||", 2);
				input++;
			} else
				add_token(tok_data, TOK_PIPE, "|", 1);
		}
		else if (*input == '&') {
			if (*(input + 1) == '&') {
				add_token(tok_data, TOK_AND, "&&", 2);
				input++;
			} else
				add_token(tok_data, TOK_AND, "&", 1);
		}
		else if (*input == '<') {
			if (*(input + 1) == '<') {
				add_token(tok_data, TOK_HEREDOC, "<<", 2);
				input++;
			} else
				add_token(tok_data, TOK_REDIR_IN, "<", 1);
		}
		else if (*input == '>') {
			if (*(input + 1) == '>') {
				add_token(tok_data, TOK_REDIR_APPEND, ">>", 2);
				input++;
			} else
				add_token(tok_data, TOK_REDIR_OUT, ">", 1);
		}
		else if (*input == '(')
			add_token(tok_data, TOK_GROUP_OPEN, "(", 1);
		else if (*input == ')')
			add_token(tok_data, TOK_GROUP_CLOSE, ")", 1);
		else if (!isspace(*input)) {
			input = process_word_token(tok_data, input);
			continue;
		}
		input++;
	}
	return tok_data->tokens;
}


