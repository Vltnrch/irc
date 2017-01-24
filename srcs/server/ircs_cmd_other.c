/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 18:20:57 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 19:36:36 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static void	ircs_cmd_nick_treat(t_ircs *ircs, char **tab, int s, char *buff)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!tab[1] || !ft_strlen(tab[1]))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Common... choose a nick !:\n");
	else if (ft_strlen(tab[1]) > MAXNICK)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Max nick's lenght is 9 !:\n");
	else if (!ft_str_isalpha(tab[1]))
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:Nick need to be only alpha !:\n");
	else if (ircs_nickexist(ircs, tab[1]) != -1)
		c_buf_write(&(fd->c_buf_send), \
					"-1:-1:-1:This nick is already in use !:\n");
	else
	{
		ft_strcpy(fd->nick, tab[1]);
		ft_printf("//Client #%d set nick to : %s\n", s, fd->nick);
		ft_sprintf(buff, "-1:-1:-1:Nick %s set:\n", fd->nick);
		c_buf_write(&(fd->c_buf_send), buff);
	}
}

void		ircs_cmd_nick(t_ircs *ircs, char **tab, int s)
{
	t_fd	*fd;
	char	buff[BUFF_PF];

	fd = &(ircs->fds[s]);
	if (ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You already set a nick !:\n");
	else
		ircs_cmd_nick_treat(ircs, tab, s, buff);
}

static void	ircs_cmd_who_treat(t_ircs *ircs, t_fd *fd, char *buff)
{
	int		i;

	ft_sprintf(buff, "-1:-1:-1:Users actually connected on %s:", \
					ircs->chan[fd->chan]);
	c_buf_write(&(fd->c_buf_send), buff);
	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == fd->chan)
		{
			c_buf_write(&(fd->c_buf_send), " ");
			c_buf_write(&(fd->c_buf_send), ircs->fds[i].nick);
		}
		i++;
	}
	c_buf_write(&(fd->c_buf_send), ":\n");
}

void		ircs_cmd_who(t_ircs *ircs, int s)
{
	t_fd	*fd;
	char	buff[BUFF_PF];

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), \
			"-1:-1:-1:You need to set a nick before !:\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), \
			"-1:-1:-1:You need choose a channel before !:\n");
	else
		ircs_cmd_who_treat(ircs, fd, buff);
}