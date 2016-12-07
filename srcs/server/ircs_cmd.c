/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircs_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:12:13 by vroche            #+#    #+#             */
/*   Updated: 2016/12/07 18:49:15 by vroche           ###   ########.fr       */
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

static void	ircs_cmd_nick(t_ircs *ircs, char *buff, int s)
{
	char	*nick;
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	nick = buff + 5;
	if (ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "[SERVER] You already set a nick !");
	else
	{
		if (!ft_strlen(nick))
			c_buf_write(&(fd->c_buf_send), "[SERVER] Common... choose a nick !");
		else if (ft_strlen(nick) > 9)
			c_buf_write(&(fd->c_buf_send), "[SERVER] Max nick's lenght is 9 !");
		else if (ircs_nickexist(ircs, nick))
			c_buf_write(&(fd->c_buf_send), "[SERVER] This nick is already in use !");
		else
		{
			ft_strcpy(fd->nick, nick);
			ft_printf("Client #%d set nick to : %s\n", s, fd->nick);
			c_buf_write(&(fd->c_buf_send), "[SERVER] Nick ");
			c_buf_write(&(fd->c_buf_send), fd->nick);
			c_buf_write(&(fd->c_buf_send), " set");
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

static void	ircs_cmd_join(t_ircs *ircs, char *buff, int s)
{
	char	*chan;
	int		num_chat;
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (!ircs_havenick(fd))
	{
		c_buf_write(&(fd->c_buf_send), "[SERVER] You need to set a nick before !");
		return ;
	}
	chan = buff + 5;
	if (!ft_strlen(chan))
		c_buf_write(&(fd->c_buf_send), "[SERVER] Set channel name...");
	else if (ft_strlen(chan) > 10)
		c_buf_write(&(fd->c_buf_send), "[SERVER] Max channel len is 10 !");
	else
	{
		if ((num_chat = ircs_chanexist(ircs, chan)) == -1)
			num_chat = ircs_chancreate(ircs, chan);
		if (num_chat == -1)
			c_buf_write(&(fd->c_buf_send), "[SERVER] Can't create this channel because full !");
		else
		{
			fd->chan = num_chat;
			c_buf_write(&(fd->c_buf_send), "[SERVER] Channel ");
			c_buf_write(&(fd->c_buf_send), chan);
			c_buf_write(&(fd->c_buf_send), " join !");
		}
	}
}

static void	ircs_cmd_leave(t_ircs *ircs, int s)
{
	t_fd	*fd;

	fd = &(ircs->fds[s]);
	if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "[SERVER] You're not in a channel...");
	else
	{
		fd->chan = -1;
		c_buf_write(&(fd->c_buf_send), "[SERVER] Channel leave !");
	}
}

static void	ircs_cmd_msg(t_ircs *ircs, char *buff, int s)
{
	t_fd	*fd;
	int		chan;
	int		i;

	fd = &(ircs->fds[s]);
	chan = fd->chan;
	if (!ircs_havenick(fd))
		c_buf_write(&(fd->c_buf_send), "[SERVER] You need to set a nick before !");
	else if (fd->chan == -1)
		c_buf_write(&(fd->c_buf_send), "[SERVER] You need choose a channel before !");
	else
	{
		i = 0;
		while (i < ircs->maxfd)
		{
			if (ircs->fds[i].type == FD_CLIENT && ircs->fds[i].chan == chan)
			{
				c_buf_write(&(ircs->fds[i].c_buf_send), "[");
				c_buf_write(&(ircs->fds[i].c_buf_send), fd->nick);
				c_buf_write(&(ircs->fds[i].c_buf_send), "] ");
				c_buf_write(&(ircs->fds[i].c_buf_send), buff);
			}
			i++;
		}
	}
}

void	ircs_cmd_check(t_ircs *ircs, int s)
{
	char	buff[BUF_SIZE_CBUF + 1];

	/*if (!c_buf_complete_cmd(&(ircs->fds[s].c_buf_recv)))
		return ;*/
	c_buf_read(&(ircs->fds[s].c_buf_recv), buff);
	if (!ft_strncmp(buff, "NICK", 4))
		ircs_cmd_nick(ircs, buff, s);
	else if (!ft_strncmp(buff, "JOIN", 4))
		ircs_cmd_join(ircs, buff, s);
	else if (!ft_strncmp(buff, "LEAVE", 5))
		ircs_cmd_leave(ircs, s);
	/*else if (!ft_strncmp(buff, "PRIVMSG"))
		ircs_cmd_mp();
	else if (!ft_strncmp(buff, "MSG"))
		ircs_cmd_msg();*/
	else
		ircs_cmd_msg(ircs, buff, s);
}
