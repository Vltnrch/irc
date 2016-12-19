/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2016/12/19 16:38:54 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_H
#define IRC_SERVER_H

# include <sys/resource.h>
# include <sys/select.h>
# include <stdlib.h>
# include <netdb.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdio.h>
# include "libft.h"
# include "ft_printf.h"
# include "circular_buffer.h"
# include "manage.h"

# define FD_FREE		0
# define FD_SERV		1
# define FD_CLIENT		2

# define BACKLOG_IRCS	42

# define CMD_MSG	0
# define CMD_NICK	1
# define CMD_JOIN	2
# define CMD_LEAVE	3
# define CMD_MP		4
# define CMD_WHO	5

# define CMD_MSG_S		"0"
# define CMD_NICK_S		"1"
# define CMD_JOIN_S		"2"
# define CMD_LEAVE_S	"3"
# define CMD_MP_S		"4"
# define CMD_WHO_S		"5"

# define MAXCHAN		20
# define MAXCHAN_NAME	10
# define MAXNICK		9

# define MAX(x,y)		((x > y) ? x : y)

typedef struct	s_fd
{
	int			type;
	char		nick[MAXNICK + 1];
	int			chan;
	t_c_buf		c_buf_recv;
	t_c_buf		c_buf_send;
}				t_fd;

typedef struct	s_ircs
{
	t_fd		*fds;
	char		*chan[MAXCHAN];
	int			port;
	int			maxfd;
	int			max;
	int			r;
	fd_set		fd_read;
	fd_set		fd_write;
}				t_ircs;

void			ircs_init_fd(t_ircs *ircs);
void			ircs_check_fd(t_ircs *ircs);

void			ircs_cmd(t_ircs *ircs, char **tab, char *buff, int s);

void			ft_perror_exit(const char *str);

#endif