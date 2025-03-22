/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:45:18 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/10 08:52:27 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

# include <stddef.h>

typedef struct s_arena {
	char	*mem;
	size_t	size;
	size_t	used;
} t_arena;

#endif
