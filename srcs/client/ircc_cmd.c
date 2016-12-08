/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:30:07 by vroche            #+#    #+#             */
/*   Updated: 2016/12/08 16:59:34 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

static void	ircs_cmd_nick(t_ircc *ircc, char **tab)
{
	c_buf_write(&(ircc->c_buf_send), CMD_NICK);
	c_buf_write(&(ircc->c_buf_send), ":");
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), tab[1]);
		c_buf_write(&(ircc->c_buf_send), ":");
	}
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

static void	ircs_cmd_join(t_ircc *ircc, char **tab)
{
	c_buf_write(&(ircc->c_buf_send), CMD_JOIN);
	c_buf_write(&(ircc->c_buf_send), ":");
	if (tab[1] && *tab[1] == '#')
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	else if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), "#");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	}
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

static void	ircs_cmd_leave(t_ircc *ircc, char **tab)
{
	c_buf_write(&(ircc->c_buf_send), CMD_LEAVE);
	c_buf_write(&(ircc->c_buf_send), ":");
	if (tab[1] && *tab[1] == '#')
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	else if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), "#");
		c_buf_write(&(ircc->c_buf_send), tab[1]);
	}
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

static void	ircs_cmd_who(t_ircc *ircc)
{
	c_buf_write(&(ircc->c_buf_send), CMD_WHO);
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

static void	ircs_cmd_mp(t_ircc *ircc, char **tab, char *line)
{
	char	*msg;

	c_buf_write(&(ircc->c_buf_send), CMD_MP);
	c_buf_write(&(ircc->c_buf_send), ":");
	if (tab[1])
	{
		c_buf_write(&(ircc->c_buf_send), tab[1]);
		c_buf_write(&(ircc->c_buf_send), ":");
		if ((msg = ft_strstr(line, tab[1])))
		{
			c_buf_write(&(ircc->c_buf_send), msg + ft_strlen(tab[1]) + 1);
			c_buf_write(&(ircc->c_buf_send), ":");
		}
	}
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

static void	ircs_cmd_msg(t_ircc *ircc, char *line)
{
	c_buf_write(&(ircc->c_buf_send), CMD_MSG);
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), line);
	c_buf_write(&(ircc->c_buf_send), ":");
	c_buf_write(&(ircc->c_buf_send), "\r\n");
}

void	ircc_cmd(t_ircc *ircc, char **tab, char *line)
{
	if (!ft_strcmp(tab[0], "/nick"))
		ircs_cmd_nick(ircc, tab);
	else if (!ft_strcmp(tab[0], "/join"))
		ircs_cmd_join(ircc, tab);
	else if (!ft_strcmp(tab[0], "/leave"))
		ircs_cmd_leave(ircc, tab);
	else if (!ft_strcmp(tab[0], "/msg"))
		ircs_cmd_mp(ircc, tab, line);
	else if (!ft_strcmp(tab[0], "/who"))
		ircs_cmd_who(ircc);
	else if (*tab[0] != '/')
		ircs_cmd_msg(ircc, line);
}
