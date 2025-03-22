/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:18:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/01/21 23:51:11 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
#define LIBFT_H

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

#ifndef MAX_FD
#define MAX_FD 1024
#endif

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_list {
  void *content;
  struct s_list *next;
} t_list;

// --- PART 1 ---

int ft_isalpha(int a);
int ft_isdigit(int a);
int ft_isalnum(int a);
int ft_isascii(int a);
int ft_isprint(int a);
size_t ft_strlen(const char *s);
void *ft_memset(void *s, int c, size_t n);
void ft_bzero(void *s, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
size_t ft_strlcpy(char *dest, const char *src, size_t destsize);
size_t ft_strlcat(char *dest, const char *src, size_t destsize);
int ft_toupper(int c);
int ft_tolower(int c);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_memchr(const void *s, int c, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
char *ft_strnstr(const char *big, const char *little, size_t len);
int ft_atoi(const char *str);
void *ft_calloc(size_t count, size_t size);
char *ft_strdup(const char *s1);

// --- PART 2 ---

char *ft_substr(const char *s, unsigned int start, size_t len);
char *ft_strjoin(const char *s1, const char *s2);
char *ft_strjoin_free(char *s1, const char *s2);
char *ft_strtrim(char const *s1, char const *set);
char **ft_split(char const *s, char c);
char *ft_itoa(int n);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
void ft_striteri(char *s, void (*f)(unsigned int, char *));
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *str, int fd);
void ft_putendl_fd(char *str, int fd);
void ft_putnbr_fd(int n, int fd);

// --- BONUS PART ---

void ft_lstadd_back(t_list **lst, t_list *new_node);
void ft_lstadd_front(t_list **lst, t_list *new_node);
t_list *ft_lstlast(t_list *lst);
t_list *ft_lstnew(void *content);
int ft_lstsize(t_list *lst);
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// --- Get_next_line ---

char *get_next_line(int fd);
char *ft_read_to_left_str(int fd, char *left_str);
char *ft_get_line(char *left_str);
char *ft_new_left_str(char *left_str);

// --- FT_PRINTF ---

int parse_conversions(va_list args, const char *format);
int ft_printf(const char *format, ...);
int count_digits(int num);
int print_char(int character);
int print_string(char *str);
int print_pointer(void *ptr);
int get_pointer_hex(unsigned long num);
int print_unsigned_int(unsigned int num);
int count_chars(char *buffer);
char *get_hex(unsigned int num, char x, char *buffer);
int print_hex(unsigned int num, char x);
int print_int(int num);

#endif
