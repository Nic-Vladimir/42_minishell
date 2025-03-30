/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matus <matus@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:42:33 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/30 12:10:27 by matus            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
#define COLORS_H

#define PURPLE "\033[38;5;141m"
#define GREEN "\033[38;5;46m"
#define RED "\033[0;31m"
#define GREY "\033[38;5;240m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define CLEAR "\r\033[K"

void    write_string(int fd, char *str);
void    fancy_write(int fd, char *str, char *color);
#endif
