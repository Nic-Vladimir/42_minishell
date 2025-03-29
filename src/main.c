/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/29 20:45:12 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ast.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <readline/history.h>

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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	char		*prompt;
	t_ast_node	*root;
	int			status;
	t_env		*env;
	t_token		*token_head;

	(void)argc;
	(void)argv;
	status = 0;
	env = init_env(envp);
	// printf("PATH: %s\n", get_env_value(env, "PATH"));
	init_sig(SIG_REAL);
	while (1)
	{
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			init_sig(SIG_VIRTUAL_CTRL_D);
			execute_exit(env);
			break;
		}
		if (check_empty(input))
		{
			free(input);
			print_transient_prompt("\n");
			continue ;
		}
		if (*input)
			add_history(input);
		print_transient_prompt(input);
		token_head = tokenize(env->tokenizer, input);
		env->tokenizer->tokens = token_head;
		// printf("Tokenized: %p, value: %s\n", (void*)env->tokenizer->tokens,
		//	env->tokenizer->tokens ? env->tokenizer->tokens->value : "NULL");
		// print_tokens(env->tokenizer);
		root = parse(env->tokenizer);
		// printf("After parse, tokens: %p\n", (void*)env->tokenizer->tokens);
		env->root = root;
		env->input = input;
		// debug_ast(root);
		status = execute_ast(env, root);
		env->tokenizer->tokens = token_head;
		free_tokens(env->tokenizer);
		// printf("root node: [%p]", root);
		free_ast(root);
		env->last_exit_code = status;
		// printf("Command return (value: %d\n", status));
		env->input = NULL;
	
		free(input);

	}
	return (0);
}
