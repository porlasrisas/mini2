/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_loop_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:06:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 18:16:39 by guigonza         ###   ########.fr       */
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
	int	i;

	if (argv && *argv)
	{
		i = 0;
		while ((*argv)[i])
			free((*argv)[i++]);
		free(*argv);
	}
	free_cmds(*cmds);
}

int	process_token_and_append(t_parse_ctx *c, int *argc, char ***argv,
		char **envp)
{
	t_token_ctx	ctx;

	ctx.token = parse_token(c->input, c->i);
	if (!ctx.token || !*ctx.token)
		return (1);
	ctx.tok_dup = ft_strdup(ctx.token);
	if (!ctx.tok_dup)
		return (1);
	ctx.should_expand = token_should_expand(ctx.token);
	ctx.processed = process_token(ctx.tok_dup, envp, ctx.should_expand,
			c->last_status);
	free(ctx.tok_dup);
	if (!ctx.processed)
		return (1);
	if (ctx.processed[0] == '\0' && !*c->current_cmd && *argc == 0)
	{
		if (ft_strncmp(ctx.token, "\"\"", 3) != 0
			&& ft_strncmp(ctx.token, "''", 3) != 0)
		{
			free(ctx.processed);
			return (1);
		}
	}
	append_or_start_cmd_ctx(c, argc, argv, ctx.processed);
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
