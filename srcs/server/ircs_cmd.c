/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:12:13 by vroche            #+#    #+#             */
/*   Updated: 2016/12/19 18:20:25 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

static int	ircs_havenick(t_fd *fd)
{
	return (ft_strlen(fd->nick));
}

static int	ircs_nickexist(t_ircs *ircs, char *nick)
{
	int	i;

	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && !ft_strcmp(ircs->fds[i].nick, nick))
			return (1);
		i++;
	}
	return (0);
}

static void	ircs_cmd_nick(t_ircs *ircs, char **tab, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You already set a nick !:\r\n");
	else
	{
		if (!tab[1] || !ft_strlen(tab[1]))
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Common... choose a nick !:\r\n");
		else if (ft_strlen(tab[1]) > 9)
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Max nick's lenght is 9 !:\r\n");
		else if (!ft_str_isalpha(tab[1]))
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Nick need to be only alpha !:\r\n");
		else if (ircs_nickexist(ircs, tab[1]))
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:This nick is already in use !:\r\n");
		else
		{
			ft_strcpy(fd->nick, tab[1]);
			ft_printf("Client #%d set nick to : %s\n", s, fd->nick);
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Nick ");
			c_buf_write(&(fd->c_buf_send), fd->nick);
			c_buf_write(&(fd->c_buf_send), " set:\r\n");
		}
	}
}

static int	ircs_chanexist(t_ircs *ircs, char *chan)
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

static int	ircs_chancreate(t_ircs *ircs, char *chan)
{
	int	i;

	i = 0;
	while (i < MAXCHAN && ircs->chan[i])
		i++;
	if (i == MAXCHAN)
		return (-1);
	ircs->chan[i] = ft_strdup(chan);
	ft_printf("Channel %s created\n", chan);
	return (i);
}

static void	ircs_cmd_leave(t_ircs *ircs, int s)
{
	t_fd	*fd;
	int		i;
	int 	chan;

	fd = &(ircs->fds[s]);
	if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You're not in a channel...:\r\n");
	else
	{
		chan = fd->chan;
		fd->chan = -1;
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Channel ");
		c_buf_write(&(fd->c_buf_send), ircs->chan[chan]);
		c_buf_write(&(fd->c_buf_send), " leave !:\r\n");
		i = 0;
		while (i < ircs->maxfd)
		{
			if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == chan)
				return ;
			i++;
		}
		ft_printf("Channel %s deleted\n", ircs->chan[chan]);
		free(ircs->chan[chan]);
		ircs->chan[chan] = NULL;
	}
}

static void	ircs_cmd_join(t_ircs *ircs, char **tab, int s)
{
	int		num_chat;
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
	{
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need to set a nick before !:\r\n");
		return ;
	}
	if (!tab[1] || !ft_strlen(tab[1]))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Set channel name...:\r\n");
	else if (ft_strlen(tab[1]) > 10)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Max channel len is 10 !:\r\n");
	else if (!ft_str_isalpha(tab[1] + 1))
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Channel need to be only alpha !:\r\n");
	else
	{
		if ((num_chat = ircs_chanexist(ircs, tab[1])) == -1)
			num_chat = ircs_chancreate(ircs, tab[1]);
		if (num_chat == -1)
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Can't create this channel because full !:\r\n");
		else
		{
			if (fd->chan >= 0)
				ircs_cmd_leave(ircs, s);
			fd->chan = num_chat;
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Channel ");
			c_buf_write(&(fd->c_buf_send), tab[1]);
			c_buf_write(&(fd->c_buf_send), " join !:\r\n");
		}
	}
}

static void	ircs_cmd_msg(t_ircs *ircs, char *buff, int s)
{
	t_fd	*fd;
	int		i;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need to set a nick before !:\r\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need choose a channel before !:\r\n");
	else
	{
		i = 0;
		while (i < ircs->maxfd)
		{
			if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == fd->chan)
			{
				c_buf_write(&(ircs->fds[i].c_buf_send), CMD_MSG_S);
				c_buf_write(&(ircs->fds[i].c_buf_send), ":");
				c_buf_write(&(ircs->fds[i].c_buf_send), fd->nick);
				c_buf_write(&(ircs->fds[i].c_buf_send), ":");
				c_buf_write(&(ircs->fds[i].c_buf_send), ircs->chan[fd->chan]);
				c_buf_write(&(ircs->fds[i].c_buf_send), ":");
				c_buf_write(&(ircs->fds[i].c_buf_send), buff + 2);
				c_buf_write(&(ircs->fds[i].c_buf_send), "\r\n");
			}
			i++;
		}
	}
}

static int	ircs_find_nick(t_ircs *ircs, char *nick)
{
	int	i;

	i = 0;
	while (i < ircs->maxfd)
	{
		if (ircs->fds[i].type == FD_CLIENT && !ft_strcmp(nick, ircs->fds[i].nick))
			return (i);
		i++;
	}
	return (-1);
}

static void	ircs_cmd_mp(t_ircs *ircs, char **tab, char *buff, int s)
{
	t_fd	*fd;
	int		user;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need to set a nick before !:\r\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need choose a channel before !:\r\n");
	else if (tab[1] && !ft_strcmp(tab[1], fd->nick))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You can't talk to you !:\r\n");
	else if (tab[1])
	{
		if ((user = ircs_find_nick(ircs, tab[1])) == -1 || (ircs->fds[user].chan != fd->chan))
			c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Can't find this user !:\r\n");
		else
		{
			if (buff[ft_strlen(buff) - 1] == ':')
				buff[ft_strlen(buff) - 1] = 0;
			c_buf_write(&(ircs->fds[user].c_buf_send), CMD_MP_S);
			c_buf_write(&(ircs->fds[user].c_buf_send), ":");
			c_buf_write(&(ircs->fds[user].c_buf_send), fd->nick);
			c_buf_write(&(ircs->fds[user].c_buf_send), ":");
			c_buf_write(&(ircs->fds[user].c_buf_send), "MP");
			c_buf_write(&(ircs->fds[user].c_buf_send), ":");
			c_buf_write(&(ircs->fds[user].c_buf_send), buff + ft_strlen(tab[0]) + ft_strlen(tab[1]) + 2);
			c_buf_write(&(ircs->fds[user].c_buf_send), ":");
			c_buf_write(&(ircs->fds[user].c_buf_send), "\r\n");
			c_buf_write(&(fd->c_buf_send), CMD_MP_S);
			c_buf_write(&(fd->c_buf_send), ":");
			c_buf_write(&(fd->c_buf_send), ircs->fds[user].nick);
			c_buf_write(&(fd->c_buf_send), ":");
			c_buf_write(&(fd->c_buf_send), "MP to");
			c_buf_write(&(fd->c_buf_send), ":");
			c_buf_write(&(fd->c_buf_send), buff + ft_strlen(tab[0]) + ft_strlen(tab[1]) + 2);
			c_buf_write(&(fd->c_buf_send), ":");
			c_buf_write(&(fd->c_buf_send), "\r\n");
		}
	}
}

static void	ircs_cmd_who(t_ircs *ircs, int s)
{
	t_fd	*fd;
	int		i;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need to set a nick before !:\r\n");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:You need choose a channel before !:\r\n");
	else
	{
		c_buf_write(&(fd->c_buf_send), "-1:-1:-1:Users actually connected on ");
		c_buf_write(&(fd->c_buf_send), ircs->chan[fd->chan]);
		c_buf_write(&(fd->c_buf_send), ": ");
		i = 0;
		while (i < ircs->maxfd)
		{
			if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == fd->chan)
			{
				c_buf_write(&(fd->c_buf_send), ircs->fds[i].nick);
				c_buf_write(&(fd->c_buf_send), " ");
			}
			i++;
		}
		c_buf_write(&(fd->c_buf_send), ":\r\n");
	}
}

void	ircs_cmd(t_ircs *ircs, char **tab, char *buff, int s)
{
	int	cmd;

	cmd = ft_atoi(tab[0]);
	if (cmd == CMD_NICK)
		ircs_cmd_nick(ircs, tab, s);
	else if (cmd == CMD_JOIN)
		ircs_cmd_join(ircs, tab, s);
	else if (cmd == CMD_LEAVE)
		ircs_cmd_leave(ircs, s);
	else if (cmd == CMD_MP)
		ircs_cmd_mp(ircs, tab, buff, s);
	else if (cmd == CMD_WHO)
		ircs_cmd_who(ircs, s);
	else if (cmd == CMD_MSG)
		ircs_cmd_msg(ircs, buff, s);
}
