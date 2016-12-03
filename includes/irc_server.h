/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2016/12/03 14:06:18 by vroche           ###   ########.fr       */
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

# define FD_FREE		0
# define FD_SERV		1
# define FD_CLIENT		2

# define BUF_SIZE		4096

# define BACKLOG_IRCS	42

# define MAX(x,y)		((x > y) ? x : y)

typedef struct	s_fd
{
	int			type;
	char		buf_read[BUF_SIZE + 1];
	char		buf_write[BUF_SIZE + 1];
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