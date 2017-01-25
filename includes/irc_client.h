/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/03 12:28:38 by vroche            #+#    #+#             */
/*   Updated: 2017/01/25 14:14:56 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_H
# define IRC_CLIENT_H

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

# define MAX(x,y)	((x > y) ? x : y)

# define CMD_SERV	"-1"
# define CMD_MSG	"0"
# define CMD_NICK	"1"
# define CMD_JOIN	"2"
# define CMD_LEAVE	"3"
# define CMD_MP		"4"
# define CMD_WHO	"5"

static char		*g_cmd[] = {
	"/help",
	"/connect",
	"/disconnect",
	"/nick",
	"/join",
	"/leave",
	"/who",
	"/msg",
	"/quit",
	NULL};

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

int				ircc_isconnected(t_ircc *ircc);
void			ircc_cmd(t_ircc *ircc, char *line);

void			ircc_cmd_join(t_ircc *ircc, char **tab);
void			ircc_cmd_leave(t_ircc *ircc, char **tab);

void			ircc_cmd_disconnect(t_ircc *ircc);
void			ircc_cmd_connect(t_ircc *ircc, char **tab);

void			ircc_cmd_mp(t_ircc *ircc, char **tab, char *line);
void			ircc_cmd_msg(t_ircc *ircc, char *line);

void			ircc_cmd_help();
void			ircc_cmd_nick(t_ircc *ircc, char **tab);
void			ircc_cmd_quit(t_ircc *ircc);
void			ircc_cmd_who(t_ircc *ircc);

t_ircc			*get_ircc_struct(void);
void			ircc_init_socket(t_ircc *ircc);
void			ircc_core(t_ircc *ircc);

void			ircc_readline(char *line);
char			**cmd_completion(const char *text, int start, int end);

void			ircc_check_fd(t_ircc *ircc);
void			ircc_init_fd(t_ircc *ircc);

#endif
