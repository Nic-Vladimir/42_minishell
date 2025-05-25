/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:12:29 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/24 11:18:31 by vnicoles         ###   ########.fr       */
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

static int	handle_wildcard_expansion(t_ast_node *node, int i)
{
	char	*temp;
	char	*args_str;
	char	**args_list;
	char	**temp_args;

	temp = node->args[i];
	args_str = expand_wildcard(temp);
	if (!args_str)
		return (1);
	args_list = ft_split(args_str, ' ');
	free(args_str);
	temp_args = copy_args(node, i, args_list);
	free_list(node->args);
	node->args = temp_args;
	ft_free_split(args_list);
	return (0);
}

static int	handle_variable_expansion(t_env *env, t_ast_node *node, int i)
{
	char	*temp;
	char	*expanded;

	temp = node->args[i];
	expanded = expand_var(env, temp);
	node->args[i] = ft_strdup(expanded);
	free(expanded);
	free(temp);
	if (!node->args[i])
		return (1);
	return (0);
}

int	execute_command_expansion(t_env *env, t_ast_node *node, int in_fd,
		int out_fd)
{
	int	i;

	i = 0;
	while (node->args[i] != NULL)
	{
		if (node->arg_types[i] == TOK_WORD && ft_strchr(node->args[i], '*'))
		{
			if (handle_wildcard_expansion(node, i) != 0)
				return (1);
		}
		else if (node->arg_types[i] == TOK_WORD
			|| node->arg_types[i] == TOK_DBQ_BLOCK)
		{
			if (handle_variable_expansion(env, node, i) != 0)
				return (1);
		}
		i++;
	}
	return (create_child_process(env, node, in_fd, out_fd));
}
