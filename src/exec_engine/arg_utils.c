/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:30:55 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:27:21 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_arg_types(t_ast_node *node, int star_index, int len_args,
		int len_expanded)
{
	int				i;
	int				j;
	t_token_type	*new_arg_types;
	int				new_size;

	new_size = (star_index) + len_expanded + (len_args - star_index - 1);
	new_arg_types = malloc(sizeof(int) * new_size);
	if (!new_arg_types)
		return ;
	ft_memset(new_arg_types, 0, sizeof(int) * new_size);
	i = 0;
	while (i < star_index)
	{
		new_arg_types[i] = node->arg_types[i];
		i++;
	}
	j = 0;
	while (j++ < len_expanded)
		new_arg_types[i++] = TOK_WORD;
	while (star_index + 1 < len_args)
		new_arg_types[i++] = node->arg_types[++star_index];
	free(node->arg_types);
	node->arg_types = new_arg_types;
}

int	calculate_total_args(t_ast_node *node, char **expanded, int *len_args,
		int *len_expanded)
{
	*len_args = 0;
	while (node->args[*len_args])
		(*len_args)++;
	*len_expanded = 0;
	while (expanded[*len_expanded])
		(*len_expanded)++;
	return (*len_args + *len_expanded);
}
