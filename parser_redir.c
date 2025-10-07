/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 20:13:55 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* helpers now live in parser_redir_utils.c */
void	attach_new_cmd_if_needed(t_parse_ctx *c, int *argc, char ***argv);
int		detect_quote_types(const char *s, int *had_single, int *had_double);
void	redir_output(t_cmd *cmd, const char *filename, int append);
void	redir_input_or_heredoc(t_cmd *cmd, const char *filename, int is_heredoc,
			int no_expand_heredoc);
char	*get_filename_noq(t_parse_ctx *c, int is_heredoc,
			int *no_expand_heredoc, int *parse_error);
char	classify_redir_and_advance(t_parse_ctx *c, int *append,
			int *is_heredoc);

void	parse_redir(t_parse_ctx *c, int *argc, char ***argv, int *parse_error)
{
	char	redir_type;
	int		append;
	int		is_heredoc;
	int		no_expand_heredoc;
	char	*filename_noq;

	redir_type = classify_redir_and_advance(c, &append, &is_heredoc);
	filename_noq = get_filename_noq(c, is_heredoc, &no_expand_heredoc,
			parse_error);
	if (!filename_noq)
		return ;
	attach_new_cmd_if_needed(c, argc, argv);
	if ((*c->current_cmd)->skip_execution)
	{
		free(filename_noq);
		return ;
	}
	if (redir_type == '>')
		redir_output(*c->current_cmd, filename_noq, append);
	else
		redir_input_or_heredoc(*c->current_cmd, filename_noq, is_heredoc,
			no_expand_heredoc);
	if (!(redir_type == '<' && is_heredoc))
		free(filename_noq);
}
