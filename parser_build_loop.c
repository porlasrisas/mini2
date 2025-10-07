/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:18:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 19:11:45 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	parse_main_loop(t_parse_ctx *c, int *argc, char ***argv, char **envp)
{
	int	st;

	while (c->input[*c->i])
	{
		st = loop_iteration(c, argc, argv, envp);
		if (st == 0)
		{
			cleanup_parse_failure(argv, c->cmds);
			return (0);
		}
		if (!skip_spaces(c))
			break ;
	}
	return (1);
}
