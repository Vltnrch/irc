/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:05:18 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 17:58:44 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	ircc_cmd_mp(t_ircc *ircc, char **tab, char *line)
{
	char	buff[BUFF_PF];

	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_MP);
	if (tab[2])
	{
		ft_sprintf(buff, ":%s:%s", tab[1], \
					line + ft_strlen(tab[0]) + ft_strlen(tab[1]) + 2);
		c_buf_write(&(ircc->c_buf_send), buff);
	}
	c_buf_write(&(ircc->c_buf_send), ":\n");
}

void	ircc_cmd_msg(t_ircc *ircc, char *line)
{
	char	buff[BUFF_PF];

	if (!ircc_isconnected(ircc))
		return ;
	ft_sprintf(buff, "%s:%s:\n", CMD_MSG, line);
	c_buf_write(&(ircc->c_buf_send), buff);
}
