/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:03:29 by vroche            #+#    #+#             */
/*   Updated: 2017/01/24 18:11:53 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGE_H
# define MANAGE_H

# include <string.h>
# include <errno.h>
# include "ft_printf.h"

char	**ft_spacesplit(const char *s);

void	ft_perror_exit(const char *str);
void	ft_perror(const char *str);
void	ft_freetab(char **tab);
int		ft_str_isalpha(char *str);

#endif
