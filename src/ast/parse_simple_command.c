/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:02:55 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:17:36 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ast.h"
#include "../../inc/minishell.h"

static int	count_command_args(t_token *tokens)
{
	int		arg_count;
	t_token	*temp;

	arg_count = 0;
	temp = tokens;
	while (temp && (temp->type == TOK_WORD || temp->type == TOK_SGQ_BLOCK
			|| temp->type == TOK_DBQ_BLOCK))
	{
		arg_count++;
		temp = temp->next;
	}
	return (arg_count);
}

static t_ast_node	*populate_command_args(t_tokenizer_data *tok_data,
		t_ast_node *cmd, char **args)
{
	int		i;
	t_token	*current;

	i = 0;
	current = tok_data->tokens;
	while (current && (current->type == TOK_WORD
			|| current->type == TOK_SGQ_BLOCK
			|| current->type == TOK_DBQ_BLOCK))
	{
		args[i] = ft_strdup(current->value);
		cmd->arg_types[i++] = current->type;
		current = current->next;
	}
	args[i] = NULL;
	tok_data->tokens = current;
	while (tok_data->tokens && (tok_data->tokens->type == TOK_REDIR_IN
			|| tok_data->tokens->type == TOK_REDIR_OUT
			|| tok_data->tokens->type == TOK_REDIR_APPEND
			|| tok_data->tokens->type == TOK_HEREDOC))
	{
		cmd = parse_redirection(tok_data, cmd);
	}
	return (cmd);
}

t_ast_node	*parse_simple_command(t_tokenizer_data *tok_data)
{
	t_ast_node	*cmd;
	char		**args;
	int			arg_count;

	arg_count = count_command_args(tok_data->tokens);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	ft_memset(args, 0, sizeof(char *) * (arg_count + 1));
	cmd = init_cmd_node(args, arg_count);
	if (!cmd)
	{
		free(args);
		return (NULL);
	}
	cmd = populate_command_args(tok_data, cmd, args);
	return (cmd);
}
