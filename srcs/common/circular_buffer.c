/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:19:10 by vroche            #+#    #+#             */
/*   Updated: 2016/12/08 15:39:02 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	c_buf_init(t_c_buf *c_buf)
{
	c_buf->read = c_buf->buf;
	c_buf->write = c_buf->buf;
}

void	c_buf_read_cmd(t_c_buf *c_buf, char *dest)
{
	int	i;
	char *tmp;

	tmp = c_buf->read;
	while (tmp != c_buf->write)
	{
		if (*tmp == '\r' && (tmp + 1) != c_buf->write && *(tmp + 1) == '\n')
			break ;
		tmp++;
		if (tmp == c_buf->buf + BUF_SIZE_CBUF)
			tmp = c_buf->buf;
	}
	while (c_buf->read != tmp)
	{
		*dest++ = *(c_buf->read)++;
		if (c_buf->read == c_buf->buf + BUF_SIZE_CBUF)
			c_buf->read = c_buf->buf;
	}
	i = 0;
	while (i < 2)
	{
		c_buf->read++;
		if (c_buf->read == c_buf->buf + BUF_SIZE_CBUF)
			c_buf->read = c_buf->buf;
		i++;
	}
	*dest = 0;
}

void	c_buf_read(t_c_buf *c_buf, char *dest)
{
	while (c_buf->read != c_buf->write)
	{
		*dest++ = *(c_buf->read)++;
		if (c_buf->read == c_buf->buf + BUF_SIZE_CBUF)
			c_buf->read = c_buf->buf;
	}
	*dest = 0;
}

void	c_buf_write(t_c_buf *c_buf, char *src)
{
	while (*src)
	{
		*(c_buf->write)++ = *src++;
		if (c_buf->write == c_buf->buf + BUF_SIZE_CBUF)
			c_buf->write = c_buf->buf;
	}
}

size_t	c_buf_len(t_c_buf *c_buf)
{
	if (c_buf->write >= c_buf->read)
		return ((size_t)(c_buf->write - c_buf->read));
	else
		return ((size_t)(BUF_SIZE_CBUF + c_buf->write - c_buf->read));
}

int		c_buf_complete_cmd(t_c_buf *c_buf)
{
	char *tmp;

	tmp = c_buf->read;
	while (tmp != c_buf->write)
	{
		if (*tmp == '\r' && (tmp + 1) != c_buf->write && *(tmp + 1) == '\n')
			return (1);
		tmp++;
		if (tmp == c_buf->buf + BUF_SIZE_CBUF)
			tmp = c_buf->buf;
	}
	return (0);
}
