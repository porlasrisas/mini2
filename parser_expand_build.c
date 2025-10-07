/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_build.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 01:16:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 12:42:02 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	read_var_end(const char *token, int i)
{
	int	end;

	end = i + 1;
	while (token[end] && (ft_isalnum(token[end]) || token[end] == '_'))
		end++;
	return (end);
}

static void	expand_status(t_expand_ctx *ctx)
{
	int	k;

	k = 0;
	while ((size_t)k < ctx->status_len)
		ctx->dst[ctx->j++] = ctx->status_str[k++];
}

static void	expand_named_var(const char *token, int *i, t_expand_ctx *ctx)
{
	int		end;
	int		len_var;
	char	*var;
	char	*value;
	int		k;

	end = read_var_end(token, *i);
	len_var = end - *i - 1;
	if (len_var == 0)
	{
		ctx->dst[ctx->j++] = '$';
		*i = end;
		return ;
	}
	var = ft_substr(token, *i + 1, len_var);
	value = get_env_var(var, ctx->envp);
	free(var);
	k = 0;
	while (value && value[k])
		ctx->dst[ctx->j++] = value[k++];
	*i = end;
}

char	*build_expand(const char *token, int len, t_expand_ctx *ctx)
{
	int	i;

	ctx->dst = (char *)malloc(sizeof(char) * (len + 1));
	if (!ctx->dst)
		return (NULL);
	i = 0;
	ctx->j = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1])
		{
			if (token[i + 1] == '?')
			{
				expand_status(ctx);
				i += 2;
			}
			else
				expand_named_var(token, &i, ctx);
		}
		else
			ctx->dst[ctx->j++] = token[i++];
	}
	ctx->dst[ctx->j] = '\0';
	return (ctx->dst);
}
