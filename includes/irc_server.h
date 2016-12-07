/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2016/12/06 17:55:26 by vroche           ###   ########.fr       */
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

void			ircs_cmd_check(t_ircs *ircs, int s);

void			ft_perror_exit(const char *str);

#endif