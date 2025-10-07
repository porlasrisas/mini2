/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 18:16:04 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* Declaraciones de helpers movidos a otro fichero */
int				parse_main_loop(t_parse_ctx *c, int *argc, char ***argv,
					char **envp);

static t_cmd	*finalize_after_loop(t_parse_ctx *c, int argc, char **argv)
{
	t_cmd	*cmds;

	cmds = *c->cmds;
	if (argv)
	{
		argv[argc] = NULL;
		if (!cmds && argc > 0)
			cmds = add_cmd(cmds, argv);
		else if (!cmds)
			free(argv);
		else if (argc > 0)
			cmds = add_cmd(cmds, argv);
	}
	if (count_cmd_list(cmds) <= 1)
		mark_non_pipeline_skips(cmds);
	return (cmds);
}

t_cmd	*parse_build(char *input, char **envp, int last_status)
{
	int			tmp[2];
	char		**argv;
	t_parse_ctx	c;
	t_cmd		*cur;
	t_cmd		*head;

	argv = (char **)malloc(sizeof(char *) * (MAX_TOKENS + 1));
	if (!argv)
		return (NULL);
	tmp[0] = 0;
	tmp[1] = 0;
	cur = NULL;
	head = NULL;
	c.input = input;
	c.i = &tmp[1];
	c.cmds = &head;
	c.current_cmd = &cur;
	c.last_status = last_status;
	if (!parse_main_loop(&c, &tmp[0], &argv, envp))
	{
		return (NULL);
	}
	head = finalize_after_loop(&c, tmp[0], argv);
	return (head);
}
