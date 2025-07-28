/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:29:00 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/19 12:45:09 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "ast.h"
# include "sig_hand.h"
# include "tokenizer.h"
# include <unistd.h>
# include <stdbool.h>

typedef enum e_execute_type
{
	RETURN,
	EXIT
} t_execute_type;

typedef struct s_bucket
{
	char				*key;
	char				*value;
	struct s_bucket		*next;
}						t_bucket;

typedef struct s_hashmap
{
	t_bucket			**buckets;
	ssize_t				size;
}						t_hashmap;

typedef struct s_env
{
	t_hashmap			*vars;
	int					last_exit_code;
	pid_t				shell_pid;
	t_tokenizer_data	*tokenizer;
	t_ast_node			*root;
	char				*input;
	t_sigenv			*sigenv;
	volatile sig_atomic_t should_exit_heredoc;
	char __padding[64 - sizeof(volatile sig_atomic_t)];
}						t_env;

#endif
