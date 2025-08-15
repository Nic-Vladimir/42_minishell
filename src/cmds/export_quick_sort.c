/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_quick_sort.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:21:26 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/15 14:25:02 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static int	partition(char **envp, int low, int high)
{
	int		i;
	int		j;
	char	*pivot;

	pivot = envp[high];
	i = low - 1;
	j = low;
	while (j <= high - 1)
	{
		if (ft_strcmp(envp[j], pivot) <= 0)
		{
			i++;
			swap(&envp[i], &envp[j]);
		}
		j++;
	}
	swap(&envp[i + 1], &envp[high]);
	return (i + 1);
}

void	sort_envp(char **envp, int low, int high)
{
	int	index;

	if (low < high)
	{
		index = partition(envp, low, high);
		sort_envp(envp, low, index - 1);
		sort_envp(envp, index + 1, high);
	}
}
