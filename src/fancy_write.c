/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fancy_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:00:27 by mgavorni          #+#    #+#             */
/*   Updated: 2025/03/31 02:01:33 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    write_string(int fd, char *str)
{
    ssize_t status;

    status = write(fd, str, ft_strlen(str));
    if (status == -1)
        return;
    status = write(fd, "\n", 1);
    if (status == -1)
        return;
}

void    fancy_write(int fd, char *str, char *color)
{
    ssize_t status;
    
    status = 0;
    if (ft_strcmp(color, RED) == 0)
        status = write(fd, RED, ft_strlen(RED));
    else if (ft_strcmp(color, PURPLE) == 0)
        status = write(fd, PURPLE, ft_strlen(PURPLE));
    else if (ft_strcmp(color, GREEN) == 0)
        status = write(fd, GREEN, ft_strlen(GREEN));
    else if (ft_strcmp(color, GREY) == 0)
        status = write(fd, GREY, ft_strlen(GREY));
    else if (ft_strcmp(color, YELLOW) == 0)
        status = write(fd, YELLOW, ft_strlen(YELLOW));
    else if (ft_strcmp(color, BLUE) == 0)
        status = write(fd, BLUE, ft_strlen(BLUE));
    if (status != -1)
    {
        write_string(fd, str);
        status = write(fd, RESET, ft_strlen(RESET));
    }
}
