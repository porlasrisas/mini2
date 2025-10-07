/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 18:46:07 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	copy_single_quoted(const char *t, int *i, char *dst, int *j)
{
	char	q;

	q = '\'';
	(*i)++;
	while (t[*i] && t[*i] != q)
		dst[(*j)++] = t[(*i)++];
	if (t[*i] == q)
		(*i)++;
}

static void	copy_double_quoted(const char *t, int *i, t_expand_ctx *x,
		int *expand_out)
{
	char	q;

	q = '"';
	(*i)++;
	while (t[*i] && t[*i] != q)
	{
		if (t[*i] == '$' && expand_out)
			*expand_out = 1;
		x->dst[x->j++] = t[(*i)++];
	}
	if (t[*i] == q)
		(*i)++;
}

static void	copy_unquoted(const char *t, int *i, char *dst, int *j)
{
	dst[(*j)++] = t[(*i)++];
}

static int	copy_without_quotes(const char *token, char *dst, int *expand_out)
{
	int				i;
	int				j;
	t_expand_ctx	x;

	i = 0;
	j = 0;
	while (token[i])
	{
		if (token[i] == '\'')
			copy_single_quoted(token, &i, dst, &j);
		else if (token[i] == '"')
		{
			x.dst = dst;
			x.j = j;
			copy_double_quoted(token, &i, &x, expand_out);
			j = x.j;
		}
		else
			copy_unquoted(token, &i, dst, &j);
	}
	return (j);
}

char	*remove_quotes_preserving_expand(char *token, int *expand_out)
{
	int		len;
	char	*result;
	int		j;

	len = ft_strlen(token);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	j = copy_without_quotes(token, result, expand_out);
	result[j] = '\0';
	return (result);
}
