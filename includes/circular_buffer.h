/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_buffer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:34:16 by vroche            #+#    #+#             */
/*   Updated: 2016/12/08 15:20:24 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

# define BUF_SIZE_CBUF	4096

typedef struct	s_c_buf
{
	char		buf[BUF_SIZE_CBUF];
	char		*read;
	char		*write;
}				t_c_buf;

void			c_buf_init(t_c_buf *c_buf);
void			c_buf_read_cmd(t_c_buf *c_buf, char *dest);
void			c_buf_read(t_c_buf *c_buf, char *src);
void			c_buf_write(t_c_buf *c_buf, char *dest);
size_t			c_buf_len(t_c_buf *c_buf);
int				c_buf_complete_cmd(t_c_buf *c_buf);
#endif
