/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:12:29 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 15:22:49 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_list(char **list)
{
	int	i;

	i = 0;
	while (list[i])
	{
		free(list[i]);
		i++;
	}
	free(list);
}

static void	handle_wildcard_expansion(t_ast_node *node, int i)
{
	char	*temp;
	char	*args_str;
	char	**args_list;
	char	**temp_args;

	temp = node->args[i];
	args_str = expand_wildcard(temp);
	if (!args_str)
		args_str = ft_strdup("");
	args_list = ft_split(args_str, ' ');
	free(args_str);
	temp_args = copy_args(node, i, args_list);
	free_list(node->args);
	node->args = temp_args;
	ft_free_split(args_list);
}

static void	handle_variable_expansion(t_env *env, t_ast_node *node, int i)
{
	char	*temp;
	char	*expanded;

	temp = node->args[i];
	expanded = expand_var(env, temp);
	node->args[i] = ft_strdup(expanded);
	free(expanded);
	free(temp);
	if (!node->args[i])
		node->args[i] = ft_strdup("");
}

void	execute_command_expansion(t_env *env, t_ast_node *node)
{
	int	i;

	if (!node || !node->args)
		return ;
	i = 0;
	while (node->args[i] != NULL)
	{
		if (node->arg_types[i] == TOK_WORD && ft_strchr(node->args[i], '*'))
			handle_wildcard_expansion(node, i);
		else if (node->arg_types[i] == TOK_WORD
			|| node->arg_types[i] == TOK_DBQ_BLOCK)
			handle_variable_expansion(env, node, i);
		i++;
	}
}
