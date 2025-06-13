/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:27:53 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/13 19:52:36 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../inc/minishell.h"

static int	status_check(int *pipe_fds, int status, char *expanded, char *line)
{
	if (status == -1)
	{
		free(expanded);
		free(line);
		close(pipe_fds[0]);
		close(pipe_fds[1]);

		return (-1);
	}
	return (0);
}
static bool herdoc_stop(char *line, char *delimiter)
{
    if (!line)
        return (true);
    if (ft_strcmp(line, delimiter) == 0)
        return (true);   
    return (false);
}
int process_heredoc_input(t_env *env, char *delimiter, int write_fd)
{
    char    *line;
    char    *expanded;
    ssize_t status = 0;

    while (1)
    {
        line = readline("heredoc> ");
        if (herdoc_stop(line, delimiter))
        {
            if (!line)
                fprintf(stderr, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
            free(line);
            break;
        }
        expanded = expand_var(env, line);
        if (expanded)
        {
            status = write(write_fd, expanded, ft_strlen(expanded));
            if (status == -1 || write(write_fd, "\n", 1) == -1)
            {
                perror("write failed");
				status = status_check(&write_fd, status, expanded, line);
                return (status);
            }
            free(expanded);
        }
        free(line);
    }
    return (0);
}

// int process_heredoc_input(t_env *env, char *delimiter, int write_fd)
// {
//     char    *line;
//     char    *expanded;
//     ssize_t status;
    
//     status = 0;
//     line = NULL;
//     while (1)
//     {
//         // if(sig > 0)
//         // {
//         //     mini_sigint_handler(sig);
//         //     return (-1);
//         // }
        
//        // rl_cleanup_after_signal();
//         line = readline("heredoc> ");
        
//         // Check if we should stop (delimiter found or EOF)
//         if (herdoc_stop(line, delimiter))
//         {
//             if (!line) // This is specifically the EOF case
//             {
//                 fprintf(stderr, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
//             }
            
//             free(line);
//             // break; // Actually break out of the loop!
//         }
//         expanded = expand_var(env, line);
//         if (expanded)
//         {
//             status = write(write_fd, expanded, ft_strlen(expanded));
//             if (status == -1 || write(write_fd, "\n", 1) == -1)
//             {
//                 status_check(NULL, status, expanded, line);
//                 return (-1);
//             }
//             free(expanded);
//         }
//         free(line);
//     }
    
//     return (status);
// }

