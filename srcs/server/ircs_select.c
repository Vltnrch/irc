/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_select.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 15:58:08 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 15:09:25 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ircs_read(t_ircs *ircs, int s)
{
	int		r;
	char	buff[BUF_SIZE_CBUF + 1];

	r = recv(s, &buff, BUF_SIZE_CBUF, 0);
	if (r <= 0)
	{
		close(s);
		ircs->fds[s].type = FD_FREE;
		ft_printf("//Client #%d gone away\n", s);
	}
	else
	{
		buff[r] = 0;
		c_buf_write(&(ircs->fds[s].c_buf_recv), buff);
		ft_printf("//Client #%d send : %s%s", s, buff, \
					buff[r - 1] == '\n' ? "" : "\n");
	}
}

static void	ircs_write(t_ircs *ircs, int s)
{
	char	buff[BUF_SIZE_CBUF + 1];

	c_buf_read(&(ircs->fds[s].c_buf_send), buff);
	send(s, buff, ft_strlen(buff), 0);
	ft_printf("//Send to #%d : %s%s", s, buff, \
					buff[ft_strlen(buff) - 1] == '\n' ? "" : "\n");
}

static void	ircs_accept(t_ircs *ircs, int s)
{
	int					ns;
	struct sockaddr_in6	nsa_in6;
	socklen_t			nsa_len;
	char				ntop[128];

	nsa_len = sizeof(struct sockaddr_in6);
	if ((ns = accept(s, (struct sockaddr*)&nsa_in6, &nsa_len)) == -1)
		return (ft_perror("accept"));
	if (ns >= ircs->maxfd)
	{
		close(ns);
		return (ft_perror("No place for new client"));
	}
	ft_printf("//New client #%d from %s port %d\n", ns, \
				inet_ntop(AF_INET6, &nsa_in6, ntop, sizeof(ntop)), \
				ntohs(nsa_in6.sin6_port));
	ft_bzero(&(ircs->fds[ns]), sizeof(t_fd));
	ircs->fds[ns].type = FD_CLIENT;
	ircs->fds[ns].chan = -1;
	c_buf_init(&(ircs->fds[ns].c_buf_recv));
	c_buf_init(&(ircs->fds[ns].c_buf_send));
	ircs_welcome_msg(&(ircs->fds[ns]));
}

void		ircs_init_fd(t_ircs *ircs)
{
	int	i;

	i = 0;
	ircs->max = 0;
	FD_ZERO(&(ircs->fd_read));
	FD_ZERO(&(ircs->fd_write));
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type != FD_FREE)
		{
			FD_SET(i, &(ircs->fd_read));
			if (c_buf_len(&(ircs->fds[i].c_buf_send)) > 0)
				FD_SET(i, &(ircs->fd_write));
			ircs->max = ((ircs->max > i) ? ircs->max : i);
		}
		i++;
	}
}

void		ircs_check_fd(t_ircs *ircs)
{
	int	i;

	i = 0;
	while ((i < ircs->maxfd) && (ircs->r > 0))
	{
		if (FD_ISSET(i, &(ircs->fd_read)))
		{
			if (ircs->fds[i].type == FD_SERV)
				ircs_accept(ircs, i);
			else
				ircs_read(ircs, i);
		}
		if (FD_ISSET(i, &(ircs->fd_write)))
			ircs_write(ircs, i);
		if (FD_ISSET(i, &(ircs->fd_read)) || FD_ISSET(i, &(ircs->fd_write)))
			ircs->r--;
		i++;
	}
}
