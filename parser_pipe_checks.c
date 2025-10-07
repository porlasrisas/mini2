/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 19:11:45 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	pipe_precheck(t_parse_ctx *c, int *argc, char ***argv)
{
	if ((*argc == 0) && (!*c->current_cmd || !(*c->current_cmd)->argv
			|| !(*c->current_cmd)->argv[0]))
	{
		ms_syntax_error("|");
		if (*argv)
			free(*argv);
		return (0);
	}
	*c->current_cmd = NULL;
	*argc = 0;
	if (*argv)
		free(*argv);
	*argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
	if (!*argv)
		return (0);
	return (1);
}

int	pipe_postcheck(t_parse_ctx *c, char ***argv)
{
	(*c->i)++;
	while (c->input[*c->i] == ' ' || c->input[*c->i] == '\t')
		(*c->i)++;
	if (c->input[*c->i] == '|')
	{
		ms_syntax_error("|");
		free(*argv);
		free_cmds(*c->cmds);
		return (0);
	}
	return (1);
}
