/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:25:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 19:56:43 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* redirecciones en el padre (builtins) */
static void	apply_parent_out(t_cmd *cmd, int *ok)
{
	if (cmd->redir.out_fd != -1)
	{
		if (dup2(cmd->redir.out_fd, STDOUT_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.out_fd);
		cmd->redir.out_fd = -1;
	}
	if (cmd->redir.append_fd != -1)
	{
		if (dup2(cmd->redir.append_fd, STDOUT_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.append_fd);
		cmd->redir.append_fd = -1;
	}
}

static void	apply_parent_in(t_cmd *cmd, int *ok)
{
	if (cmd->redir.in_fd != -1)
	{
		if (dup2(cmd->redir.in_fd, STDIN_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.in_fd);
		cmd->redir.in_fd = -1;
	}
	if (cmd->redir.heredoc_fd >= 0)
	{
		if (dup2(cmd->redir.heredoc_fd, STDIN_FILENO) == -1)
			*ok = 0;
		close(cmd->redir.heredoc_fd);
		cmd->redir.heredoc_fd = -1;
	}
}

int	apply_redirs_parent(t_cmd *cmd, int *save_stdin, int *save_stdout)
{
	int	ok;

	ok = 1;
	*save_stdin = dup(STDIN_FILENO);
	*save_stdout = dup(STDOUT_FILENO);
	if (*save_stdin == -1 || *save_stdout == -1)
		return (0);
	if (cmd->redir.has_redir_in && cmd->redir.in_fd == -1)
	{
		cmd->skip_execution = 1;
		return (0);
	}
	apply_parent_out(cmd, &ok);
	apply_parent_in(cmd, &ok);
	if (!ok)
	{
		cmd->skip_execution = 1;
		return (0);
	}
	return (ok);
}

void	restore_redirs_parent(int save_stdin, int save_stdout, t_cmd *cmd)
{
	if (save_stdin != -1)
		dup2(save_stdin, STDIN_FILENO);
	if (save_stdout != -1)
		dup2(save_stdout, STDOUT_FILENO);
	if (save_stdin != -1)
		close(save_stdin);
	if (save_stdout != -1)
		close(save_stdout);
	(void)cmd;
}

/* orquestaión delegada a executor_core.c */
int			executor_run(t_cmd *cmds, t_shell *shell);

int	execute_cmds(t_cmd *cmds, t_shell *shell)
{
	return (executor_run(cmds, shell));
}
