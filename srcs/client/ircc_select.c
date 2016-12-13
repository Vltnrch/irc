/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_select.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:08:23 by vroche            #+#    #+#             */
/*   Updated: 2016/12/13 11:43:56 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static void	ircc_print_recv(char *buff)
{
	char	**tab;

	if (!(tab = ft_strsplit(buff, ':')))
		ft_perror_exit("ft_strsplit");
	if (tab[0] && tab[1])
	{
		if (ft_atoi(tab[0]) == -1 && ft_atoi(tab[1]) == -1)
			ft_printf("[SERVER] %s\n", buff + 6);
		else
			ft_printf("%s-[%s]: %s\n", tab[1], tab[0], buff + ft_strlen(tab[0]) + ft_strlen(tab[1]) + 2);
	}
	else
		ft_printf("WTF\n");
}

static void	ircc_read(t_ircc *ircc)
{
	int		r;
	char	buff[BUF_SIZE_CBUF + 1];

	r = recv(ircc->socket, &buff, BUF_SIZE_CBUF, 0);
	if (r <= 0)
	{
		close(ircc->socket);
		rl_callback_handler_remove();
		printf("Server gone away, Bye !\n");
		exit(0);
	}
	else
	{
		buff[r - 3] = 0;
		ft_printf("\33[2K\r");
		ircc_print_recv(buff);
		rl_forced_update_display();
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
	if (FD_ISSET(ircc->socket, &(ircc->fd_read)))
		ircc_read(ircc);
	if (FD_ISSET(ircc->socket, &(ircc->fd_write)))
		ircc_write(ircc);
}

void		ircc_init_fd(t_ircc *ircc)
{
	FD_ZERO(&(ircc->fd_read));
	FD_ZERO(&(ircc->fd_write));
	FD_SET(0, &(ircc->fd_read));
	FD_SET(ircc->socket, &(ircc->fd_read));
	if (c_buf_len(&(ircc->c_buf_send)) > 0)
		FD_SET(ircc->socket, &(ircc->fd_write));
}
