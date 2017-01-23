/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2017/01/23 19:31:38 by vroche           ###   ########.fr       */
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
			if (!strcmp(line, "/quit"))
			{
				rl_callback_handler_remove();
				printf("Bye !\n");
				exit(0);
			}
			ft_printf("\33[1A\33[2K");
			ircc_prep_cmd(ircc, line);
			*rl_line_buffer = 0;
			add_history(line);
		}
		else
			ft_printf("\33[1A\33[2K");
	}
	else
		ft_printf("\33[2K\r");
}

static void	ircc_init(t_ircc *ircc)
{
	ircc->is_connected = 0;
	ircc->max = 1;
	c_buf_init(&(ircc->c_buf_recv));
	c_buf_init(&(ircc->c_buf_send));
}

void	ircc_init_socket(t_ircc *ircc)
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
	ircc->is_connected = 1;
	printf("IRC Client launch on ip : %s port : %d\n", ircc->ip, ircc->port);
}

char *g_cmd_names[] = {"/nick", "/join", "/leave", "/msg", "/who", NULL};

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

char *cmd_name_generator(const char *text, int state)
{
	static int list_index;
	static int len;
	char *name;

	if (!state) {
		list_index = 0;
		len = ft_strlen(text);
	}
	while ((name = g_cmd_names[list_index++]))
	{
		if (!ft_strncmp(name, text, len))
			return ft_strdup(name);
	}
	return (NULL);
}

char **cmd_name_completion(const char *text, int start, int end)
{
	rl_attempted_completion_over = 1;
	start = end;
	return rl_completion_matches(text, cmd_name_generator);
}

int			main(int ac, char **av)
{
	t_ircc			*ircc;

	ircc = get_ircc_struct();
	ircc->ip = "";
	ircc->port = 4242;
	ircc_init(ircc);
	if (ac > 2)
	{
		ircc->ip = av[1];
		if (av[2])
			ircc->port = ft_atoi(av[2]);
		ircc_init_socket(ircc);
		return (EXIT_FAILURE);
	}
	rl_callback_handler_install("$IRC>> ", &ircc_sendtosock);
	using_history();
	rl_attempted_completion_function = cmd_name_completion;
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
