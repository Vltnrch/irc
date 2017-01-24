/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:27:46 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 17:56:51 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static void	ircc_init(t_ircc *ircc, int ac, char **av)
{
	ircc->is_connected = 0;
	ircc->max = 1;
	c_buf_init(&(ircc->c_buf_recv));
	c_buf_init(&(ircc->c_buf_send));
	ircc->ip = ft_strdup("");
	ircc->port = 4242;
	if (ac > 1)
	{
		free(ircc->ip);
		ircc->ip = ft_strdup(av[1]);
		if (av[2])
			ircc->port = ft_atoi(av[2]);
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
