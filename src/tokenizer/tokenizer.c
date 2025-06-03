/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:40:49 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/03 19:26:30 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/tokenizer.h"

static t_token	*validate_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	int		paren_count;

	if (!tokens)
		return (NULL);
	current = tokens;
	prev = NULL;
	paren_count = 0;
	while (current && current->type != TOK_EOF)
	{
		if (!validate_single_token(current, prev, &paren_count))
			return (NULL);
		prev = current;
		current = current->next;
	}
	if (paren_count != 0)
		return (NULL);
	return (tokens);
}

t_token	*tokenize(t_tokenizer_data *tok_data, char *input)
{
	char	curr_char;

	while (*input)
	{
		curr_char = *input;
		if (curr_char == '|')
			input = process_pipe(tok_data, input);
		else if (curr_char == '&')
			input = process_ampersand(tok_data, input);
		else if (curr_char == '<')
			input = process_redir_in(tok_data, input);
		else if (curr_char == '>')
			input = process_redir_out(tok_data, input);
		else if (curr_char == '(' || curr_char == ')')
			input = process_simple_token(tok_data, input);
		else if (curr_char == '\'')
			input = process_single_quotes(tok_data, input);
		else if (curr_char == '\"')
			input = process_double_quotes(tok_data, input);
		else if (!isspace(curr_char))
			input = process_word_token(tok_data, input);
		else
			input++;
	}
	return (validate_tokens(tok_data->tokens));
}

// t_token	*tokenize(t_tokenizer_data *tok_data, char *input)
//{
//	while (*input)
//	{
//		if (*input == '|')
//		{
//			if (*(input + 1) == '|')
//			{
//				add_token(tok_data, TOK_OR, "||", 2);
//				input++;
//			}
//			else
//				add_token(tok_data, TOK_PIPE, "|", 1);
//		}
//		else if (*input == '&')
//		{
//			if (*(input + 1) == '&')
//			{
//				add_token(tok_data, TOK_AND, "&&", 2);
//				input++;
//			}
//			else
//				add_token(tok_data, TOK_AND, "&", 1);
//		}
//		else if (*input == '<')
//		{
//			if (*(input + 1) == '<')
//			{
//				add_token(tok_data, TOK_HEREDOC, "<<", 2);
//				input++;
//			}
//			else
//				add_token(tok_data, TOK_REDIR_IN, "<", 1);
//		}
//		else if (*input == '>')
//		{
//			if (*(input + 1) == '>')
//			{
//				add_token(tok_data, TOK_REDIR_APPEND, ">>", 2);
//				input++;
//			}
//			else
//				add_token(tok_data, TOK_REDIR_OUT, ">", 1);
//		}
//		else if (*input == '\'')
//		{
//			input = process_single_quotes(tok_data, input);
//			continue ;
//		}
//		else if (*input == '\"')
//		{
//			input = process_double_quotes(tok_data, input);
//			continue ;
//		}
//		else if (*input == '(')
//			add_token(tok_data, TOK_GROUP_OPEN, "(", 1);
//		else if (*input == ')')
//			add_token(tok_data, TOK_GROUP_CLOSE, ")", 1);
//		else if (!isspace(*input))
//		{
//			input = process_word_token(tok_data, input);
//			continue ;
//		}
//		input++;
//	}
//	return (tok_data->tokens);
//}
