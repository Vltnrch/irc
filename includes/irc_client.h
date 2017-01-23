/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2017/01/23 19:15:55 by vroche           ###   ########.fr       */
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
# include "readline/readline.h"
# include "readline/history.h"
# include "libft.h"
# include "ft_printf.h"
# include "circular_buffer.h"
# include "manage.h"

# define MAX(x,y)		((x > y) ? x : y)

# define CMD_SERV	"-1"
# define CMD_MSG	"0"
# define CMD_NICK	"1"
# define CMD_JOIN	"2"
# define CMD_LEAVE	"3"
# define CMD_MP		"4"
# define CMD_WHO	"5"

typedef struct	s_ircc
{
	int			port;
	int			r;
	int			max;
	int			socket;
	char		*ip;
	int			is_connected;
	fd_set		fd_read;
	fd_set		fd_write;
	t_c_buf		c_buf_recv;
	t_c_buf		c_buf_send;
}				t_ircc;

void			ircc_check_fd(t_ircc *ircc);
void			ircc_init_fd(t_ircc *ircc);

void			ircc_cmd(t_ircc *ircc, char **tab, char *line);

void			ircc_print_recv(char **tab, char *buff);

void			ircc_init_socket(t_ircc *ircc);

#endif