/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:44:51 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/19 11:33:08 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/tokenizer.h"

t_token	*new_token(t_token_type type, char *value, size_t len)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	ft_memset(token, 0, sizeof(t_token));
	token->type = type;
	token->value = malloc(len + 1);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	ft_memcpy(token->value, value, len);
	token->value[len] = '\0';
	token->next = NULL;
	return (token);
}

void	add_token(t_tokenizer_data *tok_data, t_token_type type, char *value,
		size_t len)
{
	t_token	*new;

	new = new_token(type, value, len);
	if (!new)
		return ;
	if (!tok_data->tokens)
	{
		tok_data->tokens = new;
		tok_data->tail = new;
	}
	else
	{
		tok_data->tail->next = new;
		tok_data->tail = new;
	}
}

char	*process_single_quotes(t_tokenizer_data *tok_data, char *input)
{
	char	*start;

	input++;
	start = input;
	while (*input && *input != '\'')
		input++;
	add_token(tok_data, TOK_SGQ_BLOCK, start, input - start);
	input++;
	return (input);
}

char	*process_double_quotes(t_tokenizer_data *tok_data, char *input)
{
	char	*start;

	input++;
	start = input;
	while (*input && *input != '\"')
		input++;
	add_token(tok_data, TOK_DBQ_BLOCK, start, input - start);
	input++;
	return (input);
}

char	*process_word_token(t_tokenizer_data *tok_data, char *input)
{
	char	*start;

	start = input;
	while (*input && !isspace(*input) && *input != '|' && *input != '<'
		&& *input != '>' && *input != ')')
		input++;
	add_token(tok_data, TOK_WORD, start, input - start);
	return (input);
}
