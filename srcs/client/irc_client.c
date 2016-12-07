/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2016/12/07 19:32:20 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static t_ircc	*get_ircc_struct(void)
{
	static t_ircc	*ircc = NULL;

	if (!ircc)
		if (!(ircc = (t_ircc *)malloc(sizeof(t_ircc))))
			ft_perror_exit("malloc struct ircc");
	return (ircc);
}

static void	ircc_sendtosock(char *line)
{
	t_ircc	*ircc;

	if (line && *line)
	{
		ircc = get_ircc_struct();
		c_buf_write(&(ircc->c_buf_send), line);
		if (!strcmp(line, "exit"))
		{
			rl_callback_handler_remove();
			printf("Bye !\n");
			exit(0);
		}
		*rl_line_buffer = 0;
		add_history(line);
		ft_printf("\33[1A\33[2K");
	}
	//rl_callback_handler_install ("$IRC>> ", &ircc_sendtosock);
}

static void	ircc_init_socket(t_ircc *ircc)
{
	struct sockaddr_in	sa_in;
	struct protoent		*pe;
	struct hostent		*he;

	if (!(pe = getprotobyname("tcp")))
		ft_perror_exit("getprotobyname");
	if ((ircc->socket = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0)
		ft_perror_exit("socket");
	if (!(he = gethostbyname(ircc->ip)))
		ft_perror_exit("gethostbyname");
	sa_in.sin_addr = *(struct in_addr *)he->h_addr;
	sa_in.sin_port = htons(ircc->port);
	sa_in.sin_family = AF_INET;
	if(connect(ircc->socket, (struct sockaddr *)&sa_in, sizeof(struct sockaddr)) == -1)
	    ft_perror_exit("connect");
	ircc->max = ircc->socket + 1;
	c_buf_init(&(ircc->c_buf_recv));
	c_buf_init(&(ircc->c_buf_send));
}

static void	ircc_init_fd(t_ircc *ircc)
{
	FD_ZERO(&(ircc->fd_read));
	FD_ZERO(&(ircc->fd_write));
	FD_SET(0, &(ircc->fd_read));
	FD_SET(ircc->socket, &(ircc->fd_read));
	if (c_buf_len(&(ircc->c_buf_send)) > 0)
		FD_SET(ircc->socket, &(ircc->fd_write));
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
		buff[r] = 0;
		ft_printf("\33[2K\r%s\n%s%s", buff, "$IRC>> ", rl_line_buffer);
	}
}

static void	ircc_write(t_ircc *ircc)
{
	char	buff[BUF_SIZE_CBUF + 1];

	c_buf_read(&(ircc->c_buf_send), buff);
	send(ircc->socket, buff, ft_strlen(buff), 0);
}

static void	ircc_check_fd(t_ircc *ircc)
{
	if (FD_ISSET(0, &(ircc->fd_read)))
		rl_callback_read_char();
	if (FD_ISSET(ircc->socket, &(ircc->fd_read)))
		ircc_read(ircc);
	if (FD_ISSET(ircc->socket, &(ircc->fd_write)))
		ircc_write(ircc);
}

static int	ircc_bin_tab(int a, int b)
{
	a = b;
	return (1);
}

int			main(int ac, char **av)
{
	t_ircc			*ircc;

	if (ac != 3)
	{
		printf("usage: %s <ip> <port>\n", av[0]);
		return (EXIT_FAILURE);
	}
	ircc = get_ircc_struct();
	ircc->ip = av[1];
  	ircc->port = ft_atoi(av[2]);
	ircc_init_socket(ircc);
	printf("IRC Client launch on ip : %s port : %d\n", ircc->ip, ircc->port);
	rl_callback_handler_install("$IRC>> ", &ircc_sendtosock);
	using_history();
	rl_bind_key('\t', &ircc_bin_tab);
	while (42)
	{
		ircc_init_fd(ircc);
		if ((ircc->r = select(ircc->max, &(ircc->fd_read), &(ircc->fd_write), NULL, NULL)) == -1)
			ft_perror_exit("select");
		ircc_check_fd(ircc);
	}
	rl_callback_handler_remove();
	return (EXIT_SUCCESS);
}
