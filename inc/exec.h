/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:11:02 by mgavornik         #+#    #+#             */
/*   Updated: 2025/06/11 17:11:46 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXEC_H
#define EXEC_H

#include "ast.h"
#include "env.h"

typedef struct s_command_data
{
    t_env       *env;
    t_ast_node  *node;
} t_command_data;

#endif