/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:16:04 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/05 16:18:15 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

void custom_prompt_handler(void)
{
	char *prompt;                 
    // char *first_line;               
    char *newline_pos;
    char *current_input;
	size_t len;
	
	prompt = rl_prompt;               
	// first_line = prompt;
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
        //fprintf(stderr, "Signal recognized\n");
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
        //fprintf(stderr, "CTRL-C\n");
    }
}
// 	// ssize_t status;
// 	(void)sig;
// 	rl_catch_signals = 0;
// 	sig = 1;
// 	// status = 0;
// 	if (sig)
// 	{
// 		// status = write(STDOUT_FILENO, "\n", 1);
// 		// if(status == -1)
// 		// 	g_glob_sig.sig = -1;
// 		char *prompt = rl_prompt;                 // Get full prompt string
// 		char *first_line = prompt;                // Start of first line
// 		char *newline_pos = strchr(prompt, '\n'); // Find newline
// 		char *current_input = rl_line_buffer;     // Get interrupted input
// 		write(STDOUT_FILENO, "\n", 1); // Move past partial input
// 		if (newline_pos)
// 		{
// 			size_t len = newline_pos - first_line; // Length of first line
// 			write(STDOUT_FILENO, first_line, len); // Print first line
// 			write(STDOUT_FILENO, "\n", 1);         // Newline after first line
// 		}
// 		else
// 			write(STDOUT_FILENO, "\n", 1); // Fallback if no newline
// 		// write(STDOUT_FILENO, "\n", 1);         // Blank line
// 		rl_replace_line("", 0); // Clear input buffer
// 		rl_on_new_line();       // Sync after initial newline
// 		rl_on_new_line();       // Sync after first line
// 		rl_on_new_line();       // Sync after blank line
// 		rl_redisplay();         // Redisplay full prompt
// 		if (current_input && *current_input)
// 			printf("%s", current_input);
// 				// Print interrupted input on new prompt
// 		sig = SIGINT;
// 	}
// }
// void herdoc_handler(int sig) {

//     (void)sig;
//     printf("Signal recognized\n");
//     printf("exit\n");
//     exit(EXIT_SUCCESS);
// }

void	get_env(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	fprintf(stderr, "env addr2: %p\n", env);
	//free_env(env);
	printf("here\n");
}
// void herdoc_hand(int sig) 
// {
//     struct sigaction	sa;

//     sa.sa_handler = 
//     sa.sa_flags = SA_RESTART;
//     sigaction(SIGINT, &sa, NULL);
// }
void tst_handler(int sig) {

    (void)sig;
    printf("Signal recognized0\n");
	printf("exit0\n");
    exit(0);
    
}
void	cd_handler(int sig, t_env *env)
{
	(void)sig;
	printf("Signal recognized\n");
	printf("exit\n");
	execute_exit(env);
	//(void)env;
	// if (env)
	//     free_env(env);
	// exit(EXIT_SUCCESS);
}
