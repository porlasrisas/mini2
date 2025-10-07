/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_direct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:42:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 21:07:58 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	stat_and_validate_path(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		ms_error2(path, "No such file or directory");
		_exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ms_error2(path, "is a directory");
		_exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		ms_error2(path, "Permission denied");
		_exit(126);
	}
}

static void	handle_execve_error(const char *path)
{
	if (errno == EACCES)
	{
		ms_error2(path, "Permission denied");
		_exit(126);
	}
	if (errno == ENOEXEC)
	{
		ms_error2(path, "Exec format error");
		_exit(126);
	}
	if (errno == ENOENT || errno == ENOTDIR)
	{
		ms_error2(path, "No such file or directory");
		_exit(127);
	}
	_exit(126);
}

void	exec_path_or_error(t_cmd *cmd, t_shell *shell)
{
	const char	*path;

	path = cmd->argv[0];
	stat_and_validate_path(path);
	execve(path, cmd->argv, shell->envp);
	handle_execve_error(path);
}
