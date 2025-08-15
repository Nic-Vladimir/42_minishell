/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tok_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:34:25 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 15:34:41 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/tokenizer.h"
#include <stddef.h>
#include <stdlib.h>

void	clear_tok_data(t_tokenizer_data *tok_data)
{
	t_token	*curr;
	t_token	*next;

	if (!tok_data)
		return ;
	curr = tok_data->tokens;
	while (curr)
	{
		next = curr->next;
		if (curr->value)
			free(curr->value);
		free(curr);
		curr = next;
	}
	tok_data->tokens = NULL;
	tok_data->tail = NULL;
}

void	free_tok_data(t_tokenizer_data *tok_data)
{
	t_token	*curr;
	t_token	*next;

	if (!tok_data)
		return ;
	curr = tok_data->tokens;
	while (curr)
	{
		next = curr->next;
		if (curr->value)
			free(curr->value);
		free(curr);
		curr = next;
	}
	tok_data->tokens = NULL;
	tok_data->tail = NULL;
}
