/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:19:10 by vroche            #+#    #+#             */
/*   Updated: 2016/12/05 16:01:23 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_server.h"

void	c_buf_init(t_c_buf *c_buf)
{
	c_buf->read = c_buf->buf;
	c_buf->write = c_buf->buf;
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
