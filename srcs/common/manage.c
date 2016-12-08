/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:02:48 by vroche            #+#    #+#             */
/*   Updated: 2016/12/08 17:06:08 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manage.h"

void	ft_perror_exit(const char *str)
{
	ft_printf("%s: %s\n", str, strerror(errno));
	exit(EXIT_FAILURE);
}

void	ft_freetab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int		ft_str_isalpha(char *str)
{
	while (*str)
	{
		if (!ft_isalpha(*str++))
			return (0);
	}
	return (1);
}