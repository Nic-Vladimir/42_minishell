/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:16:04 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/13 20:28:34 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

void custom_prompt_handler(void)
{
	char *prompt;                 
    char *newline_pos;
    char *current_input;
	size_t len;
	
	prompt = rl_prompt;               
	newline_pos = strchr(prompt, '\n');
	current_input = rl_line_buffer;
    write(STDOUT_FILENO, "\n", 1); 
    if (newline_pos)
    {
        len = newline_pos - prompt; 
        write(STDOUT_FILENO, prompt, len);         
		 write(STDOUT_FILENO, "\n", 1);    
    }
    else
        write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);      
	rl_on_new_line();                    
	rl_redisplay();                 
	if (current_input && *current_input)
        printf("%s", current_input);
    sig = SIGINT;
}

void mini_sigint_handler(int sig) {

    rl_catch_signals = 0;
    sig = 1;
    if (sig)
    {
		static int first_interrupt = 1;
        if (first_interrupt) 
        {
            first_interrupt = 0;
        } 
        else 
        {
            write(STDOUT_FILENO, "^C", 2);
        }
        custom_prompt_handler();
    }
}


void	get_env(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	fprintf(stderr, "env addr2: %p\n", env);
	printf("here\n");
}

void tst_handler(int sig) {

    fprintf(stderr, "tst_handlersig: %d\n", sig);
    printf("Signal recognized0\n");
	printf("exit0\n");
    exit(1);
    
}
void	cd_handler(int sig, t_env *env)
{
	(void)sig;
	printf("Signal recognized\n");
	printf("exit\n");
	execute_exit(env);
}
