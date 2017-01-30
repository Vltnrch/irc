/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 16:07:06 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 16:25:07 by vroche           ###   ########.fr       */
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
	struct protoent		*pe;
	struct addrinfo		ai;
	struct addrinfo		*r_ai;

	ircc->is_connected = 0;
	ft_bzero(&ai, sizeof(struct addrinfo));
	ai.ai_family = AF_UNSPEC;
	ai.ai_socktype = SOCK_STREAM;
	if ((getaddrinfo(ircc->ip, ircc->port, &ai, &r_ai)) != 0)
		return (ft_perror("getaddrinfo"));
	if (!r_ai)
		return (ft_perror("No result by getaddrinfo"));
	if (!(pe = getprotobyname("tcp")))
		return (ft_perror("getprotobyname"));
	if ((ircc->socket = socket(r_ai->ai_family, SOCK_STREAM, pe->p_proto)) < 0)
		return (ft_perror("socket"));
	if (connect(ircc->socket, r_ai->ai_addr, r_ai->ai_addrlen) == -1)
		return (ft_perror("connect"));
	ircc->max = ircc->socket + 1;
	ircc->is_connected = 1;
	ft_printf("Connected to: %s:%s\n", ircc->ip, ircc->port);
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

static void	ircc_core_treat(char *buff)
{
	char	*saved_line;
	int		saved_point;
	char	**tab;

	if (buff[ft_strlen(buff) - 1] == ':')
		buff[ft_strlen(buff) - 1] = 0;
	if (!(tab = ft_strsplit(buff, ':')))
		ft_perror_exit("ft_strsplit");
	saved_point = rl_point;
	saved_line = rl_copy_text(0, rl_end);
	rl_replace_line("", 0);
	rl_redisplay();
	ft_printf("\33[2K\r");
	ircc_print(tab, buff);
	rl_replace_line(saved_line, 0);
	rl_point = saved_point;
	rl_forced_update_display();
	free(saved_line);
	ft_freetab(tab);
}

void		ircc_core(t_ircc *ircc)
{
	char	buff[BUF_SIZE_CBUF + 1];

	while (c_buf_len(&(ircc->c_buf_recv)) > 0)
	{
		if (!c_buf_complete_cmd(&(ircc->c_buf_recv)))
			return ;
		c_buf_read_cmd(&(ircc->c_buf_recv), buff);
		ircc_core_treat(buff);
	}
}
