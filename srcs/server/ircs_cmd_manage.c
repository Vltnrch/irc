/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd_manage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 18:13:13 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 19:36:46 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

int	ircs_chancreate(t_ircs *ircs, char *chan)
{
	int	i;

	i = 0;
	while (i < MAXCHAN && ircs->chan[i])
		i++;
	if (i == MAXCHAN)
		return (-1);
	ircs->chan[i] = ft_strdup(chan);
	ft_printf("//Channel %s created\n", chan);
	return (i);
}

int	ircs_chanexist(t_ircs *ircs, char *chan)
{
	int	i;

	i = 0;
	while (i < MAXCHAN)
	{
		if (ircs->chan[i] && !ft_strcmp(ircs->chan[i], chan))
			return (i);
		i++;
	}
	return (-1);
}

int	ircs_nickexist(t_ircs *ircs, char *nick)
{
	int	i;

	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && \
			!ft_strcmp(nick, ircs->fds[i].nick))
			return (i);
		i++;
	}
	return (-1);
}

int	ircs_havenick(t_fd *fd)
{
	return (ft_strlen(fd->nick));
}
