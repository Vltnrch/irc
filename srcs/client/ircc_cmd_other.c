/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:01:59 by vroche            #+#    #+#             */
/*   Updated: 2017/01/27 14:18:12 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	ircc_cmd_help(void)
{
	ft_printf("/help			//Display this help\n");
	ft_printf("/connect <ip> [port]	");
	ft_printf("//Connect to server with an IP and port (default 6667)\n");
	ft_printf("/disconnect		//Disconnect\n");
	ft_printf("/nick <nickname>	//Choose a nickname\n");
	ft_printf("/join <#chan>		//Join a channel\n");
	ft_printf("/leave [#channel]	//Leave a channel\n");
	ft_printf("/who			//Show the user list on your channel\n");
	ft_printf("/list			//Show the channel list\n");
	ft_printf("/msg <nick> <message>	");
	ft_printf("//Send a PM to a user from your channel, with a message\n");
	ft_printf("/quit			//Exit this program\n");
}

void	ircc_cmd_nick(t_ircc *ircc, char **tab)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_NICK);
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), ":");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	}
	c_buf_write(&(ircc->c_buf_send), ":\n");
}

void	ircc_cmd_quit(t_ircc *ircc)
{
	rl_callback_handler_remove();
	ft_printf("Bye !\n");
	close(ircc->socket);
	exit(0);
}

void	ircc_cmd_who(t_ircc *ircc)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_WHO);
	c_buf_write(&(ircc->c_buf_send), ":\n");
}

void	ircc_cmd_list(t_ircc *ircc)
{
	if (!ircc_isconnected(ircc))
		return ;
	c_buf_write(&(ircc->c_buf_send), CMD_LIST);
	c_buf_write(&(ircc->c_buf_send), ":\n");
}
