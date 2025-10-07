/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:32:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 17:14:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	apply_out_redirs(t_cmd *cmd)
{
	if (cmd->redir.out_fd > 0)
	{
		if (dup2(cmd->redir.out_fd, STDOUT_FILENO) == -1)
			_exit(1);
		close(cmd->redir.out_fd);
	}
	if (cmd->redir.append_fd > 0)
	{
		if (dup2(cmd->redir.append_fd, STDOUT_FILENO) == -1)
			_exit(1);
		close(cmd->redir.append_fd);
	}
}

static void	apply_in_redirs(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redir.heredoc_fd == -2)
		handle_heredoc(cmd, shell);
	if (cmd->redir.has_redir_in && cmd->redir.in_fd == -1)
		_exit(1);
	if (cmd->redir.in_fd > 0)
	{
		if (dup2(cmd->redir.in_fd, STDIN_FILENO) == -1)
			_exit(1);
		close(cmd->redir.in_fd);
	}
	if (cmd->redir.heredoc_fd >= 0)
	{
		if (dup2(cmd->redir.heredoc_fd, STDIN_FILENO) == -1)
			_exit(1);
		close(cmd->redir.heredoc_fd);
	}
}

static void	apply_redirs_child(t_cmd *cmd, t_shell *shell)
{
	apply_in_redirs(cmd, shell);
	apply_out_redirs(cmd);
}

static void	exec_builtin_in_child(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->argv[0], "echo", 5) == 0)
		_exit(builtin_echo(cmd->argv));
	if (ft_strncmp(cmd->argv[0], "cd", 3) == 0)
		_exit(builtin_cd(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "pwd", 4) == 0)
		_exit(builtin_pwd());
	if (ft_strncmp(cmd->argv[0], "export", 7) == 0)
		_exit(builtin_export(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "unset", 6) == 0)
		_exit(builtin_unset(cmd->argv, shell));
	if (ft_strncmp(cmd->argv[0], "env", 4) == 0)
		_exit(builtin_env(shell->envp));
	if (ft_strncmp(cmd->argv[0], "exit", 5) == 0)
		_exit(builtin_exit(cmd->argv, shell->last_status));
}

void	exec_command_child(t_cmd *cmd, t_shell *shell, int in_pipeline)
{
	(void)in_pipeline;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	apply_redirs_child(cmd, shell);
	if (cmd->skip_execution)
		_exit(1);
	if (!cmd->argv || !cmd->argv[0])
		_exit(0);
	if (cmd->is_builtin)
		exec_builtin_in_child(cmd, shell);
	else if (ft_strchr(cmd->argv[0], '/'))
		exec_path_or_error(cmd, shell);
	else
		exec_search_in_path(cmd, shell);
}
