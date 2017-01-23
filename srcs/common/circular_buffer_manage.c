/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buffer_manage.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:43:26 by vroche            #+#    #+#             */
/*   Updated: 2017/01/23 15:43:56 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "circular_buffer.h"

void	c_buf_init(t_c_buf *c_buf)
{
	c_buf->read = c_buf->buf;
	c_buf->write = c_buf->buf;
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
		if (*tmp == '\n')
			return (1);
		tmp++;
		if (tmp == c_buf->buf + BUF_SIZE_CBUF)
			tmp = c_buf->buf;
	}
	return (0);
}
