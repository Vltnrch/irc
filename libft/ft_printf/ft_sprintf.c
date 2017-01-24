/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 11:54:27 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 12:58:36 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_spfinit(t_pf **pf, const char *format, char *str)
{
	if (!(*pf = (t_pf *)malloc(sizeof(t_pf))))
		return (-1);
	if (!((*pf)->optp = (t_optp *)malloc(sizeof(t_optp))))
		return (-1);
	(*pf)->str = str;
	(*pf)->cnt = 0;
	(*pf)->tmp = (*pf)->str;
	(*pf)->rcnt = 0;
	(*pf)->format = format;
	(*pf)->fd = 0;
	(*pf)->print = 0;
	return (0);
}

int			ft_vsprintf(char *str, const char *format, va_list ap)
{
	t_pf	*pf;
	int		rcnt;

	if (ft_spfinit(&pf, format, str) == -1)
		return (-1);
	while (*(pf->format) != '\0')
	{
		if (pf->cnt == BUFF_PF)
			break ;
		else if (*(pf->format) == '%')
			ft_perc(pf, ap);
		else
		{
			*(pf->str)++ = *(pf->format)++;
			pf->cnt++;
			pf->rcnt++;
		}
	}
	rcnt = pf->rcnt;
	str[rcnt] = 0;
	free(pf->optp);
	free(pf);
	return (rcnt);
}

int			ft_sprintf(char *str, const char *format, ...)
{
	int		r;
	va_list	*ap;

	if (!(ap = (va_list *)malloc(sizeof(va_list))))
		return (-1);
	va_start(*ap, format);
	r = ft_vsprintf(str, format, *ap);
	va_end(*ap);
	free(ap);
	return (r);
}
