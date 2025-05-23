/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:09:45 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/23 23:12:03 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/minishell.h"

void mini_sigint_handler(int sig)
{
	//ssize_t status;
	(void)sig;
	rl_catch_signals = 0;
	sig = 1;	
	//status = 0;
	if (sig)
	{
		// status = write(STDOUT_FILENO, "\n", 1);
		// if(status == -1)
		// 	g_glob_sig.sig = -1;
        char *prompt = rl_prompt;              // Get full prompt string
        char *first_line = prompt;             // Start of first line
        char *newline_pos = strchr(prompt, '\n');  // Find newline
        char *current_input = rl_line_buffer;  // Get interrupted input

        write(STDOUT_FILENO, "\n", 1);         // Move past partial input
        if (newline_pos)
        {
            size_t len = newline_pos - first_line;  // Length of first line
            write(STDOUT_FILENO, first_line, len);  // Print first line
            write(STDOUT_FILENO, "\n", 1);      // Newline after first line
        }
        else
            write(STDOUT_FILENO, "\n", 1);      // Fallback if no newline
        //write(STDOUT_FILENO, "\n", 1);         // Blank line
        rl_replace_line("", 0);                // Clear input buffer
        rl_on_new_line();                      // Sync after initial newline
        rl_on_new_line();                      // Sync after first line
        rl_on_new_line();                      // Sync after blank line
        rl_redisplay();                        // Redisplay full prompt
        if (current_input && *current_input)
            printf("%s", current_input);       // Print interrupted input on new prompt
        sig = SIGINT;
	}
}


void cd_handler(int sig)
{
	(void)sig;
	printf("Signal recognized\n");
	printf("exit\n");
	
}

