/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:29:00 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/18 19:47:39 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ENV_H
# define ENV_H

# include "arena.h"
# include "tokenizer.h"
# include <unistd.h>

typedef struct s_bucket {
	char				*key;
	char				*value;
	struct	s_bucket	*next;
} t_bucket;

typedef struct s_hashmap {
	t_bucket	**buckets;
	ssize_t		size;
} t_hashmap;

typedef struct s_env {
	t_hashmap			*vars;
	int					last_exit_code;
	pid_t				shell_pid;
	t_tokenizer_data	*tokenizer;
	t_arena				*arena;
} t_env;


#endif
