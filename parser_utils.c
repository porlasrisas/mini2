/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 18:46:30 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdlib.h>

/* forward decl from parser_expand.c */
char	*remove_quotes_preserving_expand(char *token, int *expand_out);

char	*process_token(char *token, char **envp, int expand, int last_status)
{
	char	*no_quotes;
	char	*result;
	int		expand_hint;

	expand_hint = expand;
	no_quotes = remove_quotes_preserving_expand(token, &expand_hint);
	if (!no_quotes)
		return (NULL);
	if (expand_hint)
		result = expand_env_var(no_quotes, envp, last_status);
	else
		result = ft_strdup(no_quotes);
	if (!result)
		result = ft_strdup(no_quotes);
	free(no_quotes);
	return (result);
}

int	token_should_expand(const char *token)
{
	int	in_single;
	int	in_double;
	int	j;

	in_single = 0;
	in_double = 0;
	j = 0;
	while (token[j])
	{
		if (token[j] == '\'' && !in_double)
			in_single = !in_single;
		else if (token[j] == '"' && !in_single)
			in_double = !in_double;
		else if (token[j] == '$' && !in_single)
			return (1);
		j++;
	}
	return (0);
}

void	mark_non_pipeline_skips(t_cmd *cmds)
{
	t_cmd	*cur;

	cur = cmds;
	while (cur)
	{
		if (cur->redir.has_redir_in && cur->redir.in_fd == -1)
			cur->skip_execution = 1;
		cur = cur->next;
	}
}

/* parse_token is now in parser_tokens.c */
