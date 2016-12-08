/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:03:29 by vroche            #+#    #+#             */
/*   Updated: 2016/12/08 17:03:46 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGE_H
# define MANAGE_H

# include <string.h>
# include <errno.h>
# include "ft_printf.h"

void	ft_perror_exit(const char *str);
void	ft_freetab(char **tab);
char	**ft_spacesplit(const char *s);
int		ft_str_isalpha(char *str);

#endif
