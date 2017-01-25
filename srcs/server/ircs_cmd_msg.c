/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 18:20:13 by vroche            #+#    #+#             */
/*   Updated: 2017/01/25 16:58:46 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ircs_cmd_msg_treat(t_ircs *ircs, char *buff, t_fd *fd)
{
	int		i;

	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == fd->chan)
		{
			c_buf_write(&(ircs->fds[i].c_buf_send), CMD_MSG);
			c_buf_write(&(ircs->fds[i].c_buf_send), ":");
			c_buf_write(&(ircs->fds[i].c_buf_send), fd->nick);
			c_buf_write(&(ircs->fds[i].c_buf_send), ":");
			c_buf_write(&(ircs->fds[i].c_buf_send), ircs->chan[fd->chan]);
			c_buf_write(&(ircs->fds[i].c_buf_send), ":");
			c_buf_write(&(ircs->fds[i].c_buf_send), buff + 2);
			c_buf_write(&(ircs->fds[i].c_buf_send), ":\n");
		}
		i++;
	}
}

void		ircs_cmd_msg(t_ircs *ircs, char *buff, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You need to set a nick before !:\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You need choose a channel before !:\n");
	else
		ircs_cmd_msg_treat(ircs, buff, fd);
}

static void	ircs_cmd_mp_treat(t_ircs *ircs, char **tab, char *buff, t_fd *fd)
{
	size_t	size_tab;
	int		user;

	if ((user = ircs_nickexist(ircs, tab[1])) == -1 \
		|| (ircs->fds[user].chan != fd->chan))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Can't find this user !:\n");
	else
	{
		size_tab = ft_strlen(tab[0]) + ft_strlen(tab[1]) + 2;
		c_buf_write(&(ircs->fds[user].c_buf_send), CMD_MP);
		c_buf_write(&(ircs->fds[user].c_buf_send), ":");
		c_buf_write(&(ircs->fds[user].c_buf_send), fd->nick);
		c_buf_write(&(ircs->fds[user].c_buf_send), ":PM from:");
		c_buf_write(&(ircs->fds[user].c_buf_send), buff + size_tab);
		c_buf_write(&(ircs->fds[user].c_buf_send), ":\n");
		c_buf_write(&(fd->c_buf_send), CMD_MP);
		c_buf_write(&(fd->c_buf_send), ":");
		c_buf_write(&(fd->c_buf_send), ircs->fds[user].nick);
		c_buf_write(&(fd->c_buf_send), ":PM to:");
		c_buf_write(&(fd->c_buf_send), buff + size_tab);
		c_buf_write(&(fd->c_buf_send), ":\n");
	}
}

void		ircs_cmd_mp(t_ircs *ircs, char **tab, char *buff, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You need to set a nick before !:\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:You need choose a channel before !:\n");
	else if (tab[1] && !ft_strcmp(tab[1], fd->nick))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You can't talk to you !:\n");
	else if (tab[1])
		ircs_cmd_mp_treat(ircs, tab, buff, fd);
}
