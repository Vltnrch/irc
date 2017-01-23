/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:19:10 by vroche            #+#    #+#             */
/*   Updated: 2017/01/23 15:43:52 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "circular_buffer.h"

void	c_buf_read_cmd(t_c_buf *c_buf, char *dest)
{
	char *tmp;

	tmp = c_buf->read;
	while (tmp != c_buf->write)
	{
		if (*tmp == '\n')
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
	c_buf->read++;
	if (c_buf->read == c_buf->buf + BUF_SIZE_CBUF)
		c_buf->read = c_buf->buf;
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
