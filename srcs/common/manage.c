/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:02:48 by vroche            #+#    #+#             */
/*   Updated: 2016/12/06 16:03:17 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manage.h"

void	ft_perror_exit(const char *str)
{
	ft_printf("%s: %s\n", str, strerror(errno));
	exit(EXIT_FAILURE);
}
