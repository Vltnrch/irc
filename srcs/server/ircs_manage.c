/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 14:28:37 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 15:45:06 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

t_ircs		*get_ircs_struct(void)
{
	static t_ircs	*ircs = NULL;

	if (!ircs)
		if (!(ircs = (t_ircs *)malloc(sizeof(t_ircs))))
			ft_perror_exit("malloc struct ircs");
	return (ircs);
}

static void	ircs_handler(int sig)
{
	t_ircs	*ircs;
	int		i;

	sig = 1;
	ircs = get_ircs_struct();
	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT)
			if (close(i) == -1)
				ft_perror("close");
		i++;
	}
	if (close(ircs->s) == -1)
		ft_perror("close");
	ft_printf("\nServer disconnected, Bye !\n");
	exit(0);
}

void		ircs_init_signal(void)
{
	signal(SIGINT, &ircs_handler);
	signal(SIGQUIT, &ircs_handler);
	signal(SIGTSTP, &ircs_handler);
	signal(SIGTERM, &ircs_handler);
}
