/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 01:53:11 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

/*
static void	print_tokens(t_tokenizer_data *tok_data) {
	t_tokenizer_data	*tok_ptr;

	tok_ptr = tok_data;
	while (tok_ptr->tokens) {
		printf("Tokens: [%s] (Type: %d)\n", tok_ptr->tokens->value,
			tok_ptr->tokens->type);
		tok_ptr->tokens = tok_ptr->tokens->next;
	}
}
*/
int	check_empty(const char *input)
{
	if (!input || !*input)
		return (1);
	while (*input)
	{
		if (!isspace((unsigned char)*input))
			return (0);
		input++;
	}
	return (1);
}

void	handle_command(t_env *env, char *input)
{
	t_token		*token_head;
	t_ast_node	*root;
	int			status;

	print_transient_prompt(input);
	token_head = tokenize(env->tokenizer, input);
	env->tokenizer->tokens = token_head;
	root = parse(env->tokenizer);
	env->root = root;
	env->input = input;
	env->tokenizer->tokens = token_head;
	status = execute_ast(env, root);
	free_tokens(env->tokenizer);
	free_ast(root);
	env->last_exit_code = status;
	env->input = NULL;
}

int	check_input(char *input)
{
	if (!input)
	{
		init_sig(SIG_VIRTUAL_CTRL_D);
		return (1);
	}
	if (check_empty(input))
	{
		free(input);
		print_transient_prompt("");
		return (1);
	}
	if (*input)
		add_history(input);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	init_sig(SIG_REAL);
	while (1)
	{
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (check_input(input))
			continue ;
		handle_command(env, input);
		free(input);
		input = NULL;
	}
	return (0);
}
