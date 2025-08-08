/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:36:40 by mgavornik         #+#    #+#             */
/*   Updated: 2025/08/08 17:38:03 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "fd.h"

void fd_init(my_file *fd) 
{
    ft_memset(fd, 0, sizeof(my_file));
}

my_file *fd_open(int fd, size_t buffer_size)
{
    my_file *new_file = malloc(sizeof(my_file));
    if(!new_file)
        return NULL;
    fd_init(new_file);
    new_file->fd = fd;
    new_file->readline = get_next_line;
    new_file->buffer_size = buffer_size;
    return new_file;
}

// void fd_close(my_file *fd) 
// {
//     if (fd->buffer)
//         free(fd->buffer);
//     close(fd->fd);
//     free(fd);
// }

int create_tmp_file(my_file *mf)
{
    const char *path = "/tmp/my_file";
    int tmp_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tmp_fd == -1)
        return -1;

    if (mf->tmp_path)
        free(mf->tmp_path);
    mf->tmp_path = strdup(path);
    mf->fd = tmp_fd;
    return tmp_fd;
}
int reopen_tmp_file_for_read(my_file *mf)
{
    if (!mf->tmp_path)
        return -1;
    if (mf->fd != -1)
        close(mf->fd);
    mf->fd = open(mf->tmp_path, O_RDONLY);
    if (mf->fd == -1)
        return -1;
    mf->buffer_position = 0;
    mf->buffer_end = 0;
    mf->eof = 0;
    return mf->fd;
}
void fd_close(my_file *fd) 
{
    if (fd->buffer)
        free(fd->buffer);
    if (fd->fd != -1)
        close(fd->fd);
}
