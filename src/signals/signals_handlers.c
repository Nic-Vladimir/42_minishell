/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD:src/sig_handlers.c
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:09:45 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/23 23:12:03 by mgavornik        ###   ########.fr       */
=======
/*   By: vnicoles <vnicoles@student.42prague.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:16:04 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/27 11:30:12 by vnicoles         ###   ########.fr       */
>>>>>>> origin/mvp:src/signals/signals_handlers.c
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

void mini_sigint_handler(int sig)
{
<<<<<<< HEAD:src/sig_handlers.c
	//ssize_t status;
	(void)sig;
=======
	return ((t_sig_action){
		.descript = "Ctrl-C Handler",
		.type = REAL_SIGNAL,
		.sig_handler = sig_handler,
		.sig_alt_handler = NULL,
		.virt = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = SA_RESTART,
		.signum = SIGINT});
}

t_sig_action	m_sigquit_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Ctrl-\\ Handler",
		.type = REAL_SIGNAL,
		.sig_handler = SIG_IGN,
		.sig_alt_handler = NULL,
		.virt = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = 0,
		.signum = SIGQUIT});
}

t_sig_action	m_sigchild_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Child exit Handler",
		.type = REAL_SIGNAL,
		.sig_handler = NULL,
		.sig_alt_handler = sig_alt_handler,
		.virt = NULL,
		.sig_mask = sig_mask,
		.id = SIG_REAL,
		.sig_flag = SA_SIGINFO | SA_RESTART,
		.signum = SIGCHLD});
}


void	init_sig(int id)
{
	sigset_t		sig_mask;
	size_t			i;
	t_sig_action	signals[SIGNAL_COUNT];

>>>>>>> origin/mvp:src/signals/signals_handlers.c
	rl_catch_signals = 0;
	sig = 1;	
	//status = 0;
	if (sig)
	{
<<<<<<< HEAD:src/sig_handlers.c
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
=======
		if (signals[i].type == REAL_SIGNAL && register_sig(&signals[i]) == -1)
		{
			printf("Registration of : %s FAILED\n", signals[i].descript);
		}
		else if (signals[i].type == FAKE_SIGNAL && signals[i].id == id)
		{
			signals[i].virt();
			break ;
		}
		i++;
>>>>>>> origin/mvp:src/signals/signals_handlers.c
	}
}/*
void sigquit_handler(int signum)
{
    (void)signum;
}

<<<<<<< HEAD:src/sig_handlers.c

void cd_handler(int sig)
{
	(void)sig;
	printf("Signal recognized\n");
	printf("exit\n");
	
}

=======
void sigint_handler(int signum)
{
    (void)signum;
    print_transient_prompt("");
}

void init_sig(void)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;

    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = sigquit_handler;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
*/
>>>>>>> origin/mvp:src/signals/signals_handlers.c
