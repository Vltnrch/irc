/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:12:13 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 18:21:22 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	ircs_cmd(t_ircs *ircs, char **tab, char *buff, int s)
{
	char	*cmd;

	cmd = tab[0];
	if (*cmd == *CMD_NICK)
		ircs_cmd_nick(ircs, tab, s);
	else if (*cmd == *CMD_JOIN)
		ircs_cmd_join(ircs, tab, s);
	else if (*cmd == *CMD_LEAVE)
		ircs_cmd_leave(ircs, s);
	else if (*cmd == *CMD_MP)
		ircs_cmd_mp(ircs, tab, buff, s);
	else if (*cmd == *CMD_WHO)
		ircs_cmd_who(ircs, s);
	else if (*cmd == *CMD_MSG)
		ircs_cmd_msg(ircs, buff, s);
}
