/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd_channel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 18:19:22 by vroche            #+#    #+#             */
/*   Updated: 2017/01/30 14:35:58 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ircs_cmd_leave_treat(t_ircs *ircs, t_fd *fd)
{
	int		chan;
	int		i;

	chan = fd->chan;
	fd->chan = -1;
	c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Channel ");
	c_buf_write(&(fd->c_buf_send), ircs->chan[chan]);
	c_buf_write(&(fd->c_buf_send), " leave !:\n");
	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == chan)
			return ;
		i++;
	}
	ft_printf("//Channel %s deleted\n", ircs->chan[chan]);
	free(ircs->chan[chan]);
	ircs->chan[chan] = NULL;
}

void		ircs_cmd_leave(t_ircs *ircs, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You're not in a channel...:\n");
	else
		ircs_cmd_leave_treat(ircs, fd);
}

static void	ircs_cmd_join_treat(t_ircs *ircs, char **tab, int s)
{
	int		num_chan;
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if ((num_chan = ircs_chanexist(ircs, tab[1])) == -1)
		num_chan = ircs_chancreate(ircs, tab[1]);
	if (num_chan == -1)
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:Can't create this channel because full !:\n");
	else
	{
		if (fd->chan >= 0 && num_chan != fd->chan)
			ircs_cmd_leave(ircs, s);
		if (fd->chan >= 0 && num_chan == fd->chan)
			c_buf_write(&(fd->c_buf_send), \
				"-1:-1:-1:You are already on this channel:\n");
		else
		{
			fd->chan = num_chan;
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Channel ");
			c_buf_write(&(fd->c_buf_send), tab[1]);
			c_buf_write(&(fd->c_buf_send), " join !:\n");
		}
	}
}

void		ircs_cmd_join(t_ircs *ircs, char **tab, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
	{
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You need to set a nick before !:\n");
		return ;
	}
	if (!tab[1] || !ft_strlen(tab[1]))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Set channel name...:\n");
	else if (ft_strlen(tab[1]) > MAXCHAN_NAME)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Max channel len is 10 !:\n");
	else if (*tab[1] != '#')
		c_buf_write(&(fd->c_buf_send), \
			"-1:-1:-1:Channel name must be start with a # !:\n");
	else if (!ft_str_isalpha(tab[1] + 1))
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:Channel need to be only alpha !:\n");
	else
		ircs_cmd_join_treat(ircs, tab, s);
}
