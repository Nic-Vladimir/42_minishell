/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 12:46:26 by vnicoles         ###   ########.fr       */
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
	t_token		*current;
	t_token		*next;

	(void)argc;
	(void)argv;
	status = 0;
	env = init_env(envp);
	// printf("PATH: %s\n", get_env_value(env, "PATH"));
	while (1)
	{
		kill(-1, 130);
	}
	
	init_sig(SIG_REAL);
	while (1)
	{
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			init_sig(SIG_VIRTUAL_CTRL_D);
			continue ;
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
		// env->tokenizer->tokens ? env->tokenizer->tokens->value : "NULL";
		// print_tokens(env->tokenizer);
		root = parse(env->tokenizer);
		current = token_head;
		// printf("Freeing tokens from: %p\n", (void*)tokenizer->tokens);
		printf("After parse, tokens: ");
		while (current)
		{
			next = current->next;
			printf("%s ", current->value);
			current = next;
		}
		printf("\n");
		env->root = root;
		env->input = input;
		debug_ast(root);
		env->tokenizer->tokens = token_head;
		free_tokens(&env->tokenizer->tokens);
		status = execute_ast(env, root);
		// printf("root node: [%p]", root);
		free_ast(root);
		env->last_exit_code = status;
		// printf("Command return (value: %d\n", status));
		env->input = NULL;
		free(input);
		input = NULL;
	}
	clean_rl();
	return (0);
}
