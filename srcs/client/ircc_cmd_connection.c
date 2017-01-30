/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd_connection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:03:36 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 12:22:55 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void	ircc_cmd_disconnect(t_ircc *ircc)
{
	if (!ircc_isconnected(ircc))
		return ;
	free(ircc->ip);
	free(ircc->port);
	ircc->ip = ft_strdup("");
	ircc->port = ft_strdup("6667");
	ircc->is_connected = 0;
	close(ircc->socket);
	ft_printf("Disconnected\n");
}

void	ircc_cmd_connect(t_ircc *ircc, char **tab)
{
	if (tab[1])
	{
		if (ircc->is_connected)
			ircc_cmd_disconnect(ircc);
		free(ircc->ip);
		free(ircc->port);
		ircc->ip = ft_strdup(tab[1]);
		if (tab[2])
			ircc->port = ft_strdup(tab[2]);
		else
			ircc->port = ft_strdup("6667");
		ircc_init_socket(ircc);
	}
	else
		ft_printf("Choose an IP\n");
}
