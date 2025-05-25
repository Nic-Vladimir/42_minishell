/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:29:00 by vnicoles          #+#    #+#             */
/*   Updated: 2025/05/24 17:25:11 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef ENV_H
# define ENV_H

# include "ast.h"
# include "sig_hand.h"
# include "tokenizer.h"
# include <unistd.h>

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

}						t_env;

#endif
