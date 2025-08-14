/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_filters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:54:09 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/14 13:20:28 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"
#include "../../inc/tokenizer.h"

char	*process_pipe(t_tokenizer_data *tok_data, char *input)
{
	if (*(input + 1) == '|')
	{
		add_token(tok_data, TOK_OR, "||", 2);
		input += 2;
	}
	else
	{
		add_token(tok_data, TOK_PIPE, "|", 1);
		input++;
	}
	return (input);
}

char	*process_ampersand(t_tokenizer_data *tok_data, char *input)
{
	if (*(input + 1) == '&')
	{
		add_token(tok_data, TOK_AND, "&&", 2);
		input += 2;
	}
	else
	{
		add_token(tok_data, TOK_AND, "&", 1);
		input++;
	}
	return (input);
}

char	*process_redir_in(t_tokenizer_data *tok_data, char *input)
{
	if (*(input + 1) == '<')
	{
		add_token(tok_data, TOK_HEREDOC, "<<", 2);
		input += 2;
	}
	else
	{
		add_token(tok_data, TOK_REDIR_IN, "<", 1);
		input++;
	}
	return (input);
}

char	*process_redir_out(t_tokenizer_data *tok_data, char *input)
{
	if (*(input + 1) == '>')
	{
		add_token(tok_data, TOK_REDIR_APPEND, ">>", 2);
		input += 2;
	}
	else
	{
		add_token(tok_data, TOK_REDIR_OUT, ">", 1);
		input++;
	}
	return (input);
}

char	*process_simple_token(t_tokenizer_data *tok_data, char *input)
{
	if (*input == '(')
		add_token(tok_data, TOK_GROUP_OPEN, "(", 1);
	else if (*input == ')')
		add_token(tok_data, TOK_GROUP_CLOSE, ")", 1);
	input++;
	return (input);
}

char *process_invalid_character(t_tokenizer_data *tok_data, char *input)
{
    // Optionally: print a warning or error
    fprintf(stderr, "Warning: Invalid character encountered: 0x%02x ('%c')\n", (unsigned char)*input, *input);

	//(void)tok_data;
    // Optionally: add a special token or just skip
    add_token(tok_data, TOK_INVALID, input, 1);

    // Move to the next character
    return input + 1;
}
