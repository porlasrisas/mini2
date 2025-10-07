/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 00:47:03 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <fcntl.h>

static int	has_unclosed_quotes(const char *s)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

t_cmd		*parse_build(char *input, char **envp, int last_status);

t_cmd	*parse_input(char *input, char **envp, int last_status)
{
	if (has_unclosed_quotes(input))
	{
		ms_syntax_error("newline");
		return (NULL);
	}
	return (parse_build(input, envp, last_status));
}
