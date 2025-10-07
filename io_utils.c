/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 22:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/29 22:05:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <unistd.h>

static int	grow_line(char **buf, size_t *cap)
{
	char	*tmp;
	size_t	new_cap;

	new_cap = (*cap) * 2;
	tmp = (char *)ft_realloc(*buf, *cap, new_cap);
	if (!tmp)
		return (0);
	*buf = tmp;
	*cap = new_cap;
	return (1);
}

char	*ms_read_line_fd(int fd)
{
	char	*cbuf;
	size_t	cap;
	size_t	len;
	char	c;

	cap = 64;
	len = 0;
	cbuf = (char *)malloc(cap);
	if (!cbuf)
		return (NULL);
	if (read(fd, &c, 1) <= 0)
		return (free(cbuf), NULL);
	while (c != '\n')
	{
		if (len + 1 >= cap && !grow_line(&cbuf, &cap))
			return (free(cbuf), NULL);
		cbuf[len++] = c;
		if (read(fd, &c, 1) <= 0)
			break ;
	}
	cbuf[len] = '\0';
	return (cbuf);
}
