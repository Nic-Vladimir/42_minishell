/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 20:44:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/14 02:54:48 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/ast.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

/*
static void	print_tokens(t_tokenizer_data *tok_data)
{
	t_tokenizer_data	*tok_ptr;

	tok_ptr = tok_data;
	while (tok_ptr->tokens)
	{
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
	t_token		*token_head = NULL;
	t_ast_node	*root = NULL;
	int			status = 0;

	print_transient_prompt(input);
	token_head = tokenize(env->tokenizer, input);
	if (token_head == NULL)
	{
		env->last_exit_code = 2;
		free_tokens(env->tokenizer);
		printf("minishell: Syntax error\n");
		return ;
	}
	fprintf(stderr,"[PID] handle_command() %d\n", env->shell_pid);
	env->tokenizer->tokens = token_head;
	root = parse(env->tokenizer);
	env->root = root;
	env->input = input;
	env->tokenizer->tokens = token_head;
	status = execute_ast(env, root, RETURN);
	child_comprehensive_cleanup(env);
	env->last_exit_code = status;
	env->input = NULL;
}

int	check_input(t_env *env, char *input)
{
	if (!input)
	{
		set_all_signals(CD, env->sigenv);
		cd_handler(g_sig, env);
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

int main_loop(char *prompt, char *input, t_env *env)
{

	while (1)
	{
		prompt = get_prompt(env);
		input = readline(prompt);
		free(prompt);
		if (check_input(env, input))
			continue ;
		handle_command(env, input);
		fprintf(stderr, "[PID]main_loop() %d\n", env->shell_pid); //env->shell_pid
		
		// if(env)
		// {
		// 	report_memory_usage(env);
		// 	comprehensive_cleanup(&env);
		// }
		//rl_cleanup_after_signal();
		//free(input);
		//input = NULL;	
	}
	comprehensive_cleanup(&env);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;
	t_env	*env;

	(void)argc;
	(void)argv;
	input = NULL;
	prompt = NULL;
	env = init_env(envp);
	env->sigenv->env = env;
	set_all_signals(MINI_MODE, env->sigenv);
	main_loop(prompt, input, env);
	fprintf(stderr, "[PID]main() %d\n", env->shell_pid);
	free_everything(&env);
	return (0);
}
