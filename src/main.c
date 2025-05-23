/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/23 23:13:57 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
char *mode(int mod)
{
    char *str;
    if (mod == 0)
        str = "MINI MODE";
    else if (mod == 1)
        str = "NORMAL MODE";
    else if (mod == 2)
        str = "CD MODE";
    else
        str = "UNKNOWN MODE";
    return str;
}

int	main(int argc, char **argv, char **envp) {
	char		*input;
	char		*prompt;
	t_ast_node	*root;
	int			status;
	t_env		*env;
	

	(void)argc;
	(void)argv;
	env = init_env(envp);
	env->sigenv->env = env;
	//init_signal_handlers();
	set_all_signals(MINI_MODE, env->sigenv);
	fprintf(stderr, "[MAIN BEGIN] current mode : %s\n", mode(env->sigenv->current_mode));
	while (1) 
	{
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (!input)
		{
			set_all_signals(CD, env->sigenv);
			fprintf(stderr, "current mode !input : %s\n", mode(env->sigenv->current_mode));
			cd_handler(sig);
			free_env(env);
			//free + break;
		}
		if (ft_strcmp(input, "exit") == 0) 
		{
            free(input);
			break;
        }
		if (*input)
			add_history(input);
		
		fprintf(stderr, "[MAIN FURTHER] current mode : %s\n", mode(env->sigenv->current_mode));
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
