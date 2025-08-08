/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:30:05 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/08 17:08:50 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef FD_H
# define FD_H

# include "minishell.h"


typedef struct 
{
    int fd;
    char *buffer;
    size_t buffer_size;
    size_t buffer_position;
    size_t buffer_end;
    int eof;
    char *(*readline)(int fd);
    char *tmp_path;
    
} my_file;

// Function declarations
void fd_init(my_file *fd);
my_file *fd_open(int fd, size_t buffer_size);
void fd_close(my_file *fd);
int create_tmp_file(my_file *mf);
int reopen_tmp_file_for_read(my_file *mf);

#endif