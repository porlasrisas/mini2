/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_loop_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:06:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 14:09:33 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	skip_spaces(t_parse_ctx *c)
{
	while (c->input[*c->i] == ' ' || c->input[*c->i] == '\t')
		(*c->i)++;
	if (!c->input[*c->i])
		return (0);
	return (1);
}

void	cleanup_parse_failure(char ***argv, t_cmd **cmds)
{
	if (argv && *argv)
		free(*argv);
	free_cmds(*cmds);
}

int	process_token_and_append(t_parse_ctx *c, int *argc, char ***argv,
		char **envp)
{
	char	*token;
	char	*tok_dup;
	int		should_expand;
	char	*processed;

	token = parse_token(c->input, c->i);
	if (!token || !*token)
		return (1);
	tok_dup = ft_strdup(token);
	if (!tok_dup)
		return (1);
	should_expand = token_should_expand(token);
	processed = process_token(tok_dup, envp, should_expand, c->last_status);
	free(tok_dup);
	if (!processed)
		return (1);
	if (processed[0] == '\0' && !*c->current_cmd && *argc == 0)
	{
		free(processed);
		return (1);
	}
	append_or_start_cmd_ctx(c, argc, argv, processed);
	return (1);
}

int	handle_pipe_or_redir(t_parse_ctx *c, int *argc, char ***argv,
		int *parse_error)
{
	if (c->input[*c->i] == '|')
	{
		if (!pipe_precheck(c, argc, argv))
			return (0);
		if (!pipe_postcheck(c, argv))
			return (0);
		return (2);
	}
	if (c->input[*c->i] == '<' || c->input[*c->i] == '>')
	{
		parse_redir(c, argc, argv, parse_error);
		if (*parse_error)
			return (0);
		return (2);
	}
	return (1);
}

int	loop_iteration(t_parse_ctx *c, int *argc, char ***argv, char **envp)
{
	int	st;
	int	parse_error;

	parse_error = 0;
	if (!skip_spaces(c))
		return (2);
	st = handle_pipe_or_redir(c, argc, argv, &parse_error);
	if (st == 0)
		return (0);
	if (st == 2)
		return (1);
	process_token_and_append(c, argc, argv, envp);
	return (1);
}
