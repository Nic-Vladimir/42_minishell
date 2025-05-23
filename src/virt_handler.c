/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virt_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 21:19:38 by mgavorni          #+#    #+#             */
/*   Updated: 2025/05/23 16:12:34 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/minishell.h"

void	virtual(void)
{
	printf("Signal recognized\n");
	printf("exit\n");
	exit(0);
}

// t_sig_action	m_virtual_handler(sigset_t sig_mask)
// {
// 	return ((t_sig_action){
// 		.descript = "Ctrl-D Handler",
// 		.type = FAKE_SIGNAL,
// 		.sig_handler = NULL,
// 		.sig_alt_handler = NULL,
// 		.virtual = virtual,
// 		.sig_mask = sig_mask,
// 		.id = SIG_VIRTUAL_CTRL_D,
// 		.sig_flag = 0,
// 		.signum = 0
// 	});
// }
