/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircc_readline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vroche <vroche@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 16:46:27 by vroche            #+#    #+#             */
/*   Updated: 2017/01/25 18:11:32 by vroche           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_client.h"

void		ircc_readline(char *line)
{
	t_ircc	*ircc;

	if (line)
	{
		ft_printf("\33[1A\33[2K");
		if (*line)
		{
			ircc = get_ircc_struct();
			ircc_cmd(ircc, line);
			*rl_line_buffer = 0;
			add_history(line);
		}
	}
}

static char	*ircc_cmd_generator(const char *text, int state)
{
	static int	list_index;
	static int	len;
	char		*name;

	if (!state)
	{
		list_index = 0;
		len = ft_strlen(text);
	}
	while ((name = g_cmd[list_index++]))
	{
		if (!ft_strncmp(name, text, len))
			return (ft_strdup(name));
	}
	return (NULL);
}

char		**cmd_completion(const char *text, int start, int end)
{
	rl_attempted_completion_over = 1;
	start = end;
	return (rl_completion_matches(text, ircc_cmd_generator));
}
