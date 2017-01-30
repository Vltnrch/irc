/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 15:18:12 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static void	ircc_handler(int sig)
{
	t_ircc	*ircc;

	sig = 1;
	ircc = get_ircc_struct();
	rl_callback_handler_remove();
	ft_printf("Bye !\n");
	close(ircc->socket);
	exit(0);
}

void		ircc_init_signal(void)
{
	signal(SIGINT, &ircc_handler);
	signal(SIGQUIT, &ircc_handler);
	signal(SIGTSTP, &ircc_handler);
	signal(SIGTERM, &ircc_handler);
}

static void	ircc_init(t_ircc *ircc, int ac, char **av)
{
	ircc->is_connected = 0;
	ircc->max = 1;
	c_buf_init(&(ircc->c_buf_recv));
	c_buf_init(&(ircc->c_buf_send));
	ircc->ip = ft_strdup("");
	ircc->port = ft_strdup("6667");
	if (ac > 1)
	{
		free(ircc->ip);
		free(ircc->port);
		ircc->ip = ft_strdup(av[1]);
		if (av[2])
			ircc->port = ft_strdup(av[2]);
		else
			ircc->port = ft_strdup("6667");
		ircc_init_socket(ircc);
	}
	rl_callback_handler_install("$IRC>> ", &ircc_readline);
	using_history();
	rl_attempted_completion_function = cmd_completion;
}

int			main(int ac, char **av)
{
	t_ircc			*ircc;

	ircc = get_ircc_struct();
	ircc_init(ircc, ac, av);
	ircc_init_signal();
	while (42)
	{
		ircc_init_fd(ircc);
		if ((ircc->r = select(ircc->max, &(ircc->fd_read), \
							&(ircc->fd_write), NULL, NULL)) == -1)
			ft_perror_exit("select");
		ircc_check_fd(ircc);
		ircc_core(ircc);
	}
	rl_callback_handler_remove();
	return (EXIT_SUCCESS);
}
