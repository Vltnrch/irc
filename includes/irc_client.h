/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2016/12/05 20:10:56 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_H
#define IRC_SERVER_H

# include <sys/resource.h>
# include <sys/select.h>
# include <stdlib.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "ft_printf.h"
# include "circular_buffer.h"

# define MAX(x,y)		((x > y) ? x : y)

typedef struct	s_ircc
{
	int			port;
	int			r;
	int			max;
	int			socket;
	char		*ip;
	fd_set		fd_read;
	fd_set		fd_write;
	t_c_buf		c_buf_recv;
	t_c_buf		c_buf_send;
}				t_ircc;

#endif