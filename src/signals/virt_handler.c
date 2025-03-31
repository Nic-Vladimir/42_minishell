/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virt_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavorni <mgavorni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:14:22 by vnicoles          #+#    #+#             */
/*   Updated: 2025/03/31 02:36:36 by mgavorni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	virtual(void)
{
	printf("exit\n");
	clean_rl();
	exit(0);
}

t_sig_action	m_virtual_handler(sigset_t sig_mask)
{
	return ((t_sig_action){
		.descript = "Ctrl-D Handler",
		.type = FAKE_SIGNAL,
		.sig_handler = NULL,
		.sig_alt_handler = NULL,
		.virt = virtual,
		.sig_mask = sig_mask,
		.id = SIG_VIRTUAL_CTRL_D,
		.sig_flag = 0,
		.signum = 0
	});
}
