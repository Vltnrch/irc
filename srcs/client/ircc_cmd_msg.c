/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:05:18 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 14:55:05 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	ircc_cmd_mp(t_ircc *ircc, char **tab, char *line)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_MP);
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), ":");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
		if (tab[2])
		{
			c_buf_write(&(ircc->c_buf_send), ":");
			while (ft_isspace(*line))
				line++;
			line += ft_strlen(tab[0]);
			while (ft_isspace(*line))
				line++;
			line += ft_strlen(tab[1]);
			while (ft_isspace(*line))
				line++;
			c_buf_write(&(ircc->c_buf_send), line);
		}
	}
	c_buf_write(&(ircc->c_buf_send), ":\n");
}

void	ircc_cmd_msg(t_ircc *ircc, char *line)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_MSG);
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), line);
	c_buf_write(&(ircc->c_buf_send), ":\n");
}
