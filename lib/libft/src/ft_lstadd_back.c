/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vnicoles <vnicoles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:22:45 by vnicoles          #+#    #+#             */
/*   Updated: 2025/01/21 23:50:33 by vnicoles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	last = ft_lstlast(*lst);
	if (!lst || !new_node)
		return ;
	if (!last)
	{
		*lst = new_node;
		return ;
	}
	last->next = new_node;
}
