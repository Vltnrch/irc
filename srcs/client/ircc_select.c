/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_select.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:08:23 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 16:57:28 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static void	ircc_read(t_ircc *ircc)
{
	int		r;
	char	buff[BUF_SIZE_CBUF + 1];

	r = recv(ircc->socket, &buff, BUF_SIZE_CBUF, 0);
	if (r <= 0)
	{
		close(ircc->socket);
		free(ircc->ip);
		ircc->ip = ft_strdup("");
		ircc->port = 4242;
		ircc->is_connected = 0;
		ft_printf("\33[2K\r");
		ft_printf("You're disconnected from the server !\n");
		rl_forced_update_display();
	}
	else
	{
		buff[r] = 0;
		c_buf_write(&(ircc->c_buf_recv), buff);
	}
}

static void	ircc_write(t_ircc *ircc)
{
	char	buff[BUF_SIZE_CBUF + 1];

	c_buf_read(&(ircc->c_buf_send), buff);
	send(ircc->socket, buff, ft_strlen(buff), 0);
}

void		ircc_check_fd(t_ircc *ircc)
{
	if (FD_ISSET(0, &(ircc->fd_read)))
		rl_callback_read_char();
	if (ircc->is_connected)
	{
		if (FD_ISSET(ircc->socket, &(ircc->fd_read)))
			ircc_read(ircc);
		if (FD_ISSET(ircc->socket, &(ircc->fd_write)))
			ircc_write(ircc);
	}
}

void		ircc_init_fd(t_ircc *ircc)
{
	FD_ZERO(&(ircc->fd_read));
	FD_ZERO(&(ircc->fd_write));
	FD_SET(0, &(ircc->fd_read));
	if (ircc->is_connected)
	{
		FD_SET(ircc->socket, &(ircc->fd_read));
		if (c_buf_len(&(ircc->c_buf_send)) > 0)
			FD_SET(ircc->socket, &(ircc->fd_write));
	}
}
