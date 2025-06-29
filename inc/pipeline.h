/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 02:28:19 by vnicoles          #+#    #+#             */
/*   Updated: 2025/06/29 02:54:33 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_pipeline
{
	t_ast_node	**commands;
	int			*pipefds;
	pid_t		*pids;
	int			count;
}				t_pipeline;


int		init_pipeline(t_ast_node *node, t_pipeline *pl);
int		setup_pipes(t_pipeline *pl);
int		manage_children(t_env *env, t_pipeline *pl, int in_fd, int out_fd);
void	free_pipeline(t_pipeline *pl);
