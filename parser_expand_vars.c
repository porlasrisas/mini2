/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_vars.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 10:47:12 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Split: this file now only exposes expand_env_var; helpers moved to
 * parser_expand_calc.c and parser_expand_build.c to satisfy 42 Norm.
 */

#include "includes/minishell.h"
#include "libft/libft.h"
#include <stddef.h>
#include <stdlib.h>

/* Forward decls from split units */
int		calc_len_expand(const char *token, char **envp, size_t status_len);
char	*build_expand(const char *token, int len, t_expand_ctx *ctx);

char	*expand_env_var(char *token, char **envp, int last_status)
{
	int				len;
	char			*status_str;
	size_t			status_len;
	char			*result;
	t_expand_ctx	ctx;

	if (!envp)
		return (ft_strdup(token));
	status_str = ft_itoa(last_status);
	if (!status_str)
		status_str = ft_strdup("0");
	status_len = ft_strlen(status_str);
	len = calc_len_expand(token, envp, status_len);
	ctx.envp = envp;
	ctx.status_str = status_str;
	ctx.status_len = status_len;
	result = build_expand(token, len, &ctx);
	free(status_str);
	if (!result)
		return (ft_strdup(token));
	return (result);
}
