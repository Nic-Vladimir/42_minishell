/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:19:35 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/04 20:04:56 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_arena	*arena_init(size_t size) {
	t_arena	*arena;

	arena = malloc(sizeof(t_arena));
	if (!arena)
		return NULL;
	arena->mem = malloc(size);
	if (!arena->mem) {
		free(arena);
		return NULL;
	}
	arena->size = size;
	arena->used = 0;
	return arena;
}

static int	arena_expand(t_arena *arena) {
	char	*new_mem;

	new_mem = realloc(arena->mem, arena->size * 2);
	if (!new_mem)
		return 0;
	arena->mem = new_mem;
	arena->size *= 2;
	return 1;
}

void	*arena_malloc(t_arena *arena, size_t size) {
	void	*ptr;

	while (arena->used + size > arena->size) {
		if	(!arena_expand(arena))
			return NULL;
	}
	ptr = arena->mem + arena->used;
	arena->used += size;
	return ptr;
}

void	arena_reset(t_arena *arena) {
	arena->used = 0;
}

void	arena_free(t_arena *arena) {
	if (!arena)
		return;
	free(arena->mem);
	free(arena);
}
