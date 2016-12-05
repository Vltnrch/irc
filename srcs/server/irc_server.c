/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2016/12/05 19:45:03 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ft_perror_exit(const char *str)
{
	dprintf(2, "Error : %s\n", str);
	exit(EXIT_FAILURE);
}

static void	ircs_init_struct(t_ircs *ircs, char **av)
{
	int				i;
	struct rlimit	rl;

	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
		ft_perror_exit("getrlimit");
	ircs->maxfd = rl.rlim_cur;
	if (!(ircs->fds = (t_fd *)malloc(sizeof(t_fd) * ircs->maxfd)))
		ft_perror_exit("malloc fds");
	i = 0;
	while (i < ircs->maxfd)
	{
		ircs->fds[i].type = FD_FREE;
		i++;
	}
  	ircs->port = ft_atoi(av[1]);
}

static void	ircs_init_socket(t_ircs *ircs)
{
  	int					s;
	struct sockaddr_in	sa_in;
	struct protoent		*pe;
	
	if (!(pe = getprotobyname("tcp")))
		ft_perror_exit("getprotobyname");
	if ((s = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0)
		ft_perror_exit("socket");
	sa_in.sin_family = AF_INET;
	sa_in.sin_addr.s_addr = INADDR_ANY;
	sa_in.sin_port = htons(ircs->port);
	if (bind(s, (struct sockaddr *)&sa_in, sizeof(struct sockaddr_in)) == -1)
		ft_perror_exit("bind");
	if (listen(s, BACKLOG_IRCS) == -1)
		ft_perror_exit("listen");
	ircs->fds[s].type = FD_SERV;
}

static void	ircs_init_fd(t_ircs *ircs)
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
			ircs->max = MAX(ircs->max, i);
		}
		i++;
	}
}

static void	ircs_read(t_ircs *ircs, int s)
{
	int		r;
	int		i;
	char	buff[BUF_SIZE_CBUF + 1];

	r = recv(s, &buff, BUF_SIZE_CBUF, 0);
	if (r <= 0)
	{
		close(s);
		ircs->fds[s].type = FD_FREE;
		printf("client #%d gone away\n", s);
	}
	else
	{
		buff[r] = 0;
		c_buf_write(&(ircs->fds[s].c_buf_recv), buff);
		printf("client #%d send : %s\n", s, buff);
		i = 0;
		while (i < ircs->maxfd)
		{
			if ((ircs->fds[i].type == FD_CLIENT) && (i != s))
				c_buf_write(&(ircs->fds[i].c_buf_send), buff);
			i++;
		}
	}
}

static void	ircs_write(t_ircs *ircs, int s)
{
	char	buff[BUF_SIZE_CBUF + 1];

	c_buf_read(&(ircs->fds[s].c_buf_send), buff);
	send(s, buff, ft_strlen(buff), 0);
	printf("send to #%d : %s\n", s, buff);
}

static void	ircs_accept(t_ircs *ircs, int s)
{
	int					ns;
	struct sockaddr_in	nsa_in;
	socklen_t			nsa_len;

	nsa_len = sizeof(struct sockaddr_in);
	if ((ns = accept(s, (struct sockaddr*)&nsa_in, &nsa_len)) == -1)
		ft_perror_exit("accept");
	printf("New client #%d from %s:%d\n", ns, inet_ntoa(nsa_in.sin_addr), ntohs(nsa_in.sin_port));
	ircs->fds[ns].type = FD_CLIENT;
	c_buf_init(&(ircs->fds[ns].c_buf_recv));
	c_buf_init(&(ircs->fds[ns].c_buf_send));
}

static void	ircs_check_fd(t_ircs *ircs)
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

int			main(int ac, char **av)
{
	t_ircs			ircs;

	if (ac != 2)
	{
		printf("usage: %s <port>\n", av[0]);
		return (EXIT_FAILURE);
	}
	ircs_init_struct(&ircs, av);
	ircs_init_socket(&ircs);
	printf("IRC Server launch on port : %d\nNumber on fd max : %d\n", ircs.port, ircs.maxfd);
	while (42)
	{
		ircs_init_fd(&ircs);
		if ((ircs.r = select(ircs.max + 1, &(ircs.fd_read), &(ircs.fd_write), NULL, NULL)) == -1)
			ft_perror_exit("select");
		ircs_check_fd(&ircs);
	}
	return (EXIT_SUCCESS);
}