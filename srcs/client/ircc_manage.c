/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 16:07:06 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 18:03:02 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

t_ircc		*get_ircc_struct(void)
{
	static t_ircc	*ircc = NULL;

	if (!ircc)
		if (!(ircc = (t_ircc *)malloc(sizeof(t_ircc))))
			ft_perror_exit("malloc struct ircc");
	return (ircc);
}

void		ircc_init_socket(t_ircc *ircc)
{
	struct sockaddr_in	sa_in;
	struct protoent		*pe;
	struct hostent		*he;

	ircc->is_connected = 0;
	if (!(pe = getprotobyname("tcp")))
		ft_perror_exit("getprotobyname");
	if ((ircc->socket = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0)
		ft_perror_exit("socket");
	if (!(he = gethostbyname(ircc->ip)))
		ft_perror_exit("gethostbyname");
	sa_in.sin_addr = *(struct in_addr *)he->h_addr;
	sa_in.sin_port = htons(ircc->port);
	sa_in.sin_family = AF_INET;
	if (connect(ircc->socket, (struct sockaddr *)&sa_in, \
				sizeof(struct sockaddr)) == -1)
		return (ft_perror("connect"));
	ircc->max = ircc->socket + 1;
	ircc->is_connected = 1;
	ft_printf("Connected to: %s:%d\n", ircc->ip, ircc->port);
}

static void	ircc_print(char **tab, char *buff)
{
	size_t	size_tab;

	if (tab[0] && tab[1] && tab[2])
	{
		size_tab = ft_strlen(tab[0]) + ft_strlen(tab[1]) \
					+ ft_strlen(tab[2]) + 3;
		if (ft_atoi(tab[0]) == -1 && ft_atoi(tab[1]) == -1 \
			&& ft_atoi(tab[2]) == -1)
			ft_printf("\33[32m[SERVER] %s\n", buff + size_tab);
		else
		{
			if (ft_atoi(tab[0]) == ft_atoi(CMD_MP))
				ft_printf("\33[35m");
			ft_printf("%s [%s]: %s\n", tab[2], tab[1], buff + size_tab);
		}
		ft_printf("\33[39m");
	}
	else
		ft_printf("WRONG PACKET : %s\n", buff);
}

void		ircc_core(t_ircc *ircc)
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
		ircc_print(tab, buff);
		rl_forced_update_display();
		ft_freetab(tab);
	}
}
