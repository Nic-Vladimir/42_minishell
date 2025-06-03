/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:30:55 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/03 18:53:31 by vnicoles         ###   ########.fr       */
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
/*
static void	copy_args_sections(char **args_copy, t_ast_node *node,
		int star_index, char **expanded, int len_args, int len_expanded)
{
	int	i;
	int	j;

	i = 0;
	while (i < star_index)
	{
		args_copy[i] = ft_strdup(node->args[i]);
		i++;
	}
	j = 0;
	while (j < len_expanded)
	{
		args_copy[i] = ft_strdup(expanded[j]);
		i++;
		j++;
	}
	while (star_index + 1 < len_args)
	{
		args_copy[i] = ft_strdup(node->args[star_index + 1]);
		i++;
		star_index++;
	}
	args_copy[i] = NULL;
}

char	**copy_args(t_ast_node *node, int star_index, char **expanded)
{
	char	**args_copy;
	int		len_expanded;
	int		len_args;
	int		total_size;

	if (!node || !node->args || !expanded)
		return (NULL);
	total_size = calculate_total_args(node, expanded, &len_args, &len_expanded);
	args_copy = malloc(sizeof(char *) * total_size);
	if (!args_copy)
		return (NULL);
	copy_args_sections(args_copy, node, star_index, expanded, len_args,
		len_expanded);
	update_arg_types(node, star_index, len_args, len_expanded);
	return (args_copy);
}
*/
// char	**copy_args(t_ast_node *node, int star_index, char **expanded)
//{
//	char	**args_copy;
//	int		i;
//	int		j;
//	int		len_expanded;
//	int		len_args;
//
//	if (!node || !node->args || !expanded)
//		return (NULL);
//	// Count total original args
//	len_args = 0;
//	while (node->args[len_args])
//		len_args++;
//	// Count expanded args
//	len_expanded = 0;
//	while (expanded[len_expanded])
//		len_expanded++;
//	// Allocate enough space: args before * + expanded + args after *
//	args_copy = malloc(sizeof(char *) * (len_args + len_expanded));
//	if (!args_copy)
//		return (NULL);
//	// Copy args before the * (star_index)
//	i = 0;
//	while (i < star_index)
//	{
//		args_copy[i] = ft_strdup(node->args[i]);
//		i++;
//	}
//	// Copy expanded args
//	j = 0;
//	while (j < len_expanded)
//	{
//		args_copy[i] = ft_strdup(expanded[j]);
//		i++;
//		j++;
//	}
//	// Copy remaining args after the *
//	while (star_index + 1 < len_args)
//	{
//		args_copy[i] = ft_strdup(node->args[star_index + 1]);
//		i++;
//		star_index++;
//	}
//	args_copy[i] = NULL; // Null-terminate
//	// Update node->arg_types
//	update_arg_types(node, star_index, len_args, len_expanded);
//	return (args_copy);
//}
