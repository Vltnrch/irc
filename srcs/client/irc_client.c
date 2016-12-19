/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2016/12/19 18:20:15 by vroche           ###   ########.fr       */
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

static void	ircc_prep_cmd(t_ircc *ircc, char *line)
{
	char	**tab;

	if (!(tab = ft_spacesplit(line)))
		ft_perror_exit("ft_spacesplit");
	if (tab[0] && *tab[0])
		ircc_cmd(ircc, tab, line);
	ft_freetab(tab);
}

static void	ircc_sendtosock(char *line)
{
	t_ircc	*ircc;

	if (line)
	{
		if (*line)
		{
			ircc = get_ircc_struct();
			if (!strcmp(line, "exit"))
			{
				rl_callback_handler_remove();
				printf("Bye !\n");
				exit(0);
			}
			ircc_prep_cmd(ircc, line);
			*rl_line_buffer = 0;
			add_history(line);
		}
		ft_printf("\33[1A\33[2K");
	}
	else
		ft_printf("\33[2K\r");
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

static int	ircc_bin_tab(int a, int b)
{
	a = b;
	return (1);
}

static void	ircc_core(t_ircc *ircc)
{
	char	**tab;
	char	buff[BUF_SIZE_CBUF + 1];

	while (c_buf_len(&(ircc->c_buf_recv)) > 0)
	{
		if (!c_buf_complete_cmd(&(ircc->c_buf_recv)))
			return ;
		c_buf_read_cmd(&(ircc->c_buf_recv), buff);
		if (buff[ft_strlen(buff) - 1] == ':')
			buff[ft_strlen(buff) - 1] = 0;
		if (!(tab = ft_strsplit(buff, ':')))
			ft_perror_exit("ft_strsplit");
		ft_printf("\33[2K\r");
		ircc_print_recv(tab, buff);
		rl_forced_update_display();
		ft_freetab(tab);
	}
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
		ircc_core(ircc);
	}
	rl_callback_handler_remove();
	return (EXIT_SUCCESS);
}
