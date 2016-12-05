/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2016/12/05 16:04:08 by vroche           ###   ########.fr       */
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
# include "circular_buffer.h"

# define FD_FREE		0
# define FD_SERV		1
# define FD_CLIENT		2

# define BACKLOG_IRCS	42

# define MAX(x,y)		((x > y) ? x : y)

typedef struct	s_fd
{
	int			type;
	t_c_buf		c_buf_recv;
	t_c_buf		c_buf_send;
}				t_fd;

typedef struct	s_ircs
{
	t_fd		*fds;
	int			port;
	int			maxfd;
	int			max;
	int			r;
	fd_set		fd_read;
	fd_set		fd_write;
}				t_ircs;

#endif