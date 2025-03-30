/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matus <matus@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:10:05 by matus             #+#    #+#             */
/*   Updated: 2025/03/30 12:14:15 by matus            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    write_string(int fd, char *str)
{
    write(fd, str, ft_strlen(str));
    write(fd, "\n", 1);
}

void    fancy_write(int fd, char *str, char *color)
{
    if (ft_strcmp(color, RED) == 0)
        write(fd, RED, ft_strlen(RED));
    else if(ft_strcmp(color, PURPLE) == 0)
        write(fd, PURPLE, ft_strlen(PURPLE));
    else if(ft_strcmp(color, GREEN) == 0)
        write(fd, GREEN, ft_strlen(GREEN));
    else if(ft_strcmp(color, GREY) == 0)
        write(fd, GREY, ft_strlen(GREY));
    else if(ft_strcmp(color, YELLOW) == 0)
        write(fd, YELLOW, ft_strlen(YELLOW));
    else if(ft_strcmp(color, BLUE) == 0)
        write(fd, BLUE, ft_strlen(BLUE));
    write_string(fd, str);
    write(fd, RESET, ft_strlen(RESET));
}

