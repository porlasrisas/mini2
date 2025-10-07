/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 21:07:58 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static char	*join_path(const char *dir, const char *base)
{
	char	*res;
	size_t	dl;
	size_t	bl;

	dl = ft_strlen(dir);
	bl = ft_strlen(base);
	res = malloc(dl + bl + 2);
	if (!res)
		return (NULL);
	ft_strlcpy(res, dir, dl + 1);
	res[dl] = '/';
	res[dl + 1] = '\0';
	ft_strlcat(res, base, dl + bl + 2);
	return (res);
}

static void	free_split(char **p)
{
	int	i;

	i = 0;
	while (p && p[i])
		free(p[i++]);
	free(p);
}

static int	attempt_exec(char *cand, t_cmd *cmd, t_shell *shell)
{
	execve(cand, cmd->argv, shell->envp);
	if (errno == EACCES)
		return (126);
	return (-1);
}

static int	loop_path_parts(char **parts, t_cmd *cmd, t_shell *shell)
{
	char	*cand;
	int		i;
	int		code;

	i = 0;
	while (parts[i])
	{
		cand = join_path(parts[i], cmd->argv[0]);
		if (!cand)
			return (1);
		if (access(cand, X_OK) == 0)
		{
			code = attempt_exec(cand, cmd, shell);
			free(cand);
			if (code == 126)
				return (126);
			return (127);
		}
		free(cand);
		i++;
	}
	return (127);
}

void	exec_search_in_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**parts;
	int		code;

	path = get_env_var("PATH", shell->envp);
	if (!path || path[0] == '\0')
	{
		ms_error2(cmd->argv[0], "command not found");
		_exit(127);
	}
	parts = ft_split(path, ':');
	if (!parts)
		_exit(1);
	code = loop_path_parts(parts, cmd, shell);
	if (code == 1)
		_exit(1);
	if (code == 126)
	{
		free_split(parts);
		_exit(126);
	}
	free_split(parts);
	ms_error2(cmd->argv[0], "command not found");
	_exit(127);
}
