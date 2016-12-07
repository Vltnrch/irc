/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2016/12/06 18:43:30 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ircs_init_struct(t_ircs *ircs, char **av)
{
	int				i;
	struct rlimit	rl;

	if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
		ft_perror_exit("getrlimit");
	ircs->maxfd = rl.rlim_cur;
	if (!(ircs->fds = (t_fd *)malloc(sizeof(t_fd) * ircs->maxfd)))
		ft_perror_exit("malloc fds");
	ft_bzero(ircs->fds, sizeof(t_fd) * ircs->maxfd);
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

static void	ircs_core(t_ircs *ircs)
{
	int	i;

	i = 0;
	while (i < ircs->maxfd)
	{
		if ((ircs->fds[i].type == FD_CLIENT) && (c_buf_len(&(ircs->fds[i].c_buf_recv)) > 0))
			ircs_cmd_check(ircs, i);
		i++;
	}
}

int			main(int ac, char **av)
{
	t_ircs			ircs;

	if (ac != 2)
	{
		ft_printf("usage: %s <port>\n", av[0]);
		return (EXIT_FAILURE);
	}
	ircs_init_struct(&ircs, av);
	ircs_init_socket(&ircs);
	ft_printf("IRC Server launch on port : %d\nNumber on fd max : %d\n", ircs.port, ircs.maxfd);
	while (42)
	{
		ircs_init_fd(&ircs);
		if ((ircs.r = select(ircs.max + 1, &(ircs.fd_read), &(ircs.fd_write), NULL, NULL)) == -1)
			ft_perror_exit("select");
		ircs_check_fd(&ircs);
		ircs_core(&ircs);
	}
	return (EXIT_SUCCESS);
}