/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_calc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 01:15:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 17:51:59 by Guille           ###   ########.fr       */
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

static int	len_for_var(const char *token, int i, char **envp)
{
	int		end;
	int		len_var;
	char	*var;
	char	*value;

	end = read_var_end(token, i);
	len_var = end - i - 1;
	if (len_var == 0)
		return (1);
	var = ft_substr(token, i + 1, len_var);
	value = get_env_var(var, envp);
	free(var);
	if (value)
		return ((int)ft_strlen(value));
	return (0);
}

static int	handle_dollar(const char *token, int *i, char **envp,
		size_t status_len)
{
	int	add;

	if (token[*i + 1] == '?')
	{
		*i += 2;
		return ((int)status_len);
	}
	else
	{
		add = len_for_var(token, *i, envp);
		*i = read_var_end(token, *i);
		return (add);
	}
}

static void	advance_or_count(const char *token, int *i, int *len)
{
	if (token[*i])
	{
		(*len)++;
		(*i)++;
	}
}

int	calc_len_expand(const char *token, char **envp, size_t status_len)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1])
			len += handle_dollar(token, &i, envp, status_len);
		else
			advance_or_count(token, &i, &len);
	}
	return (len);
}
