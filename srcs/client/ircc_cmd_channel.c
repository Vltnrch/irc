/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd_channel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:04:25 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 17:07:37 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	ircc_cmd_join(t_ircc *ircc, char **tab)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_JOIN);
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), ":");
		if (*tab[1] != '#')
			c_buf_write(&(ircc->c_buf_send), "#");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	}
	c_buf_write(&(ircc->c_buf_send), ":\n");
}

void	ircc_cmd_leave(t_ircc *ircc, char **tab)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_LEAVE);
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), ":");
		if (*tab[1] != '#')
			c_buf_write(&(ircc->c_buf_send), "#");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	}
	c_buf_write(&(ircc->c_buf_send), ":\n");
}
