/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_hmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:16:58 by mgavorni          #+#    #+#             */
/*   Updated: 2025/08/15 14:26:34 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	free_bucket(t_bucket *new_bucket)
{
	if (!new_bucket)
		return (0);
	free(new_bucket->key);
	free(new_bucket->value);
	free(new_bucket);
	return (1);
}
