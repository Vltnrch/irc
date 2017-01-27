/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:30:07 by vroche            #+#    #+#             */
/*   Updated: 2017/01/27 14:11:12 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

int			ircc_isconnected(t_ircc *ircc)
{
	if (!ircc->is_connected)
	{
		ft_printf("You need to be connected\n");
		return (0);
	}
	return (1);
}

static void	ircc_cmd_treat(t_ircc *ircc, char **tab, char *line)
{
	if (!ft_strcmp(tab[0], "/help"))
		ircc_cmd_help();
	else if (!ft_strcmp(tab[0], "/nick"))
		ircc_cmd_nick(ircc, tab);
	else if (!ft_strcmp(tab[0], "/join"))
		ircc_cmd_join(ircc, tab);
	else if (!ft_strcmp(tab[0], "/leave"))
		ircc_cmd_leave(ircc, tab);
	else if (!ft_strcmp(tab[0], "/msg"))
		ircc_cmd_mp(ircc, tab, line);
	else if (!ft_strcmp(tab[0], "/who"))
		ircc_cmd_who(ircc);
	else if (!ft_strcmp(tab[0], "/list"))
		ircc_cmd_list(ircc);
	else if (!ft_strcmp(tab[0], "/disconnect"))
		ircc_cmd_disconnect(ircc);
	else if (!ft_strcmp(tab[0], "/connect"))
		ircc_cmd_connect(ircc, tab);
	else if (!ft_strcmp(tab[0], "/quit"))
		ircc_cmd_quit(ircc);
	else if (*tab[0] != '/')
		ircc_cmd_msg(ircc, line);
	else
		ft_printf("Command not found\n");
}

void		ircc_cmd(t_ircc *ircc, char *line)
{
	char	**tab;

	if (!(tab = ft_spacesplit(line)))
		ft_perror_exit("ft_spacesplit");
	if (tab[0])
		ircc_cmd_treat(ircc, tab, line);
	ft_freetab(tab);
}
