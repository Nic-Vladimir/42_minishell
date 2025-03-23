/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/24 12:49:24 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include "../inc/ast.h"
#include "../inc/sig_hand.h"
#include <readline/history.h>

/*
static void print_tokens(t_tokenizer_data *tok_data) {
	t_tokenizer_data	*tok_ptr;

	tok_ptr = tok_data;
	while (tok_ptr->tokens) {
		printf("Tokens: [%s] (Type: %d)\n", tok_ptr->tokens->value, tok_ptr->tokens->type);
		tok_ptr->tokens = tok_ptr->tokens->next;
	}
}
*/

int	main(int argc, char **argv, char **envp) {
	char		*input;
	char		*prompt;
	t_ast_node	*root;
	int			status;
	t_env		*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	setup_sig_handler(SIG_REAL);
	while (1) {
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			setup_sig_handler(SIG_VIRTUAL_CTRL_D);
			continue;
		if (ft_strcmp(input, "exit") == 0) {
            free(input);
			break;
        }
		if (*input)
			add_history(input);

		print_transient_prompt(input);
		env->tokenizer->tokens = tokenize(env->tokenizer, input);
		root = parse(env->tokenizer);
		debug_ast(root);
		//print_tokens(env->tokenizer);
		status = execute_ast(env, root);
		free_tokens(env->tokenizer);
		free_ast(root);
        env->last_exit_code = status;
		printf("Command return value: %d\n", status);
		free(input);
	}
	free_env(env);
    rl_clear_history();
    rl_cleanup_after_signal();
	return 0;
}
