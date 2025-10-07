/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:27:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 10:57:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	count_envp(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**alloc_new_env(int count)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * count);
	return (arr);
}

int	env_remove_at(char ***envp, int idx)
{
	int		count;
	int		i;
	int		j;
	char	**new_env;

	count = count_envp(*envp);
	new_env = alloc_new_env(count);
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != idx)
		{
			new_env[j] = (*envp)[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free((*envp)[idx]);
	free(*envp);
	*envp = new_env;
	return (0);
}
