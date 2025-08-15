/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 18:51:34 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:27:34 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	copy_before_star(char **args_copy, t_ast_node *node, int star_index)
{
	int	i;

	i = 0;
	while (i < star_index)
	{
		args_copy[i] = ft_strdup(node->args[i]);
		i++;
	}
}

static void	copy_after_star(char **args_copy, t_ast_node *node, int star_index,
		int start_pos)
{
	int	i;
	int	len_args;

	len_args = 0;
	while (node->args[len_args])
		len_args++;
	i = start_pos;
	while (star_index + 1 < len_args)
	{
		args_copy[i] = ft_strdup(node->args[star_index + 1]);
		i++;
		star_index++;
	}
	args_copy[i] = NULL;
}

static void	copy_expanded_section(char **args_copy, char **expanded,
		int start_pos, int len_expanded)
{
	int	i;
	int	j;

	i = start_pos;
	j = 0;
	while (j < len_expanded)
	{
		args_copy[i] = ft_strdup(expanded[j]);
		i++;
		j++;
	}
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
	ft_memset(args_copy, 0, sizeof(char *) * total_size);
	copy_before_star(args_copy, node, star_index);
	copy_expanded_section(args_copy, expanded, star_index, len_expanded);
	copy_after_star(args_copy, node, star_index, star_index + len_expanded);
	update_arg_types(node, star_index, len_args, len_expanded);
	return (args_copy);
}
