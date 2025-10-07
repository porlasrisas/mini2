/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:54:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 17:00:06 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	is_delim(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

char	*parse_token(const char *input, int *i)
{
	static char	buffer[1024];
	int			start;
	int			in_single;
	int			in_double;
	int			len;

	start = *i;
	in_single = 0;
	in_double = 0;
	while (input[*i])
	{
		if (!in_single && !in_double && is_delim(input[*i]))
			break ;
		if (input[*i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[*i] == '"' && !in_single)
			in_double = !in_double;
		(*i)++;
	}
	len = *i - start;
	if (len >= 1024)
		return (NULL);
	ft_strlcpy(buffer, input + start, len + 1);
	return (buffer);
}
