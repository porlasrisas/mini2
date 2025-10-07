/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 14:08:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	heredoc_sigint(int signo)
{
	char	nl;

	(void)signo;
	g_signal = SIGINT;
	if (isatty(STDIN_FILENO))
	{
		nl = '\n';
		ioctl(STDIN_FILENO, TIOCSTI, &nl);
	}
	else
		write(STDOUT_FILENO, "\n", 1);
}

void	handle_heredoc(t_cmd *cmd, t_shell *shell)
{
	struct sigaction	old_int;
	struct sigaction	new_int;
	int					assigned_fd;

	if (!(cmd->redir.heredoc_fd == -2))
		return ;
	if (!cmd->redir.heredocs && !cmd->redir.heredoc_delim)
		return ;
	new_int.sa_handler = heredoc_sigint;
	sigemptyset(&new_int.sa_mask);
	new_int.sa_flags = 0;
	sigaction(SIGINT, &new_int, &old_int);
	assigned_fd = -1;
	if (!hd_process_list(cmd, shell, &assigned_fd))
		cmd->redir.heredoc_fd = -1;
	else
		cmd->redir.heredoc_fd = assigned_fd;
	sigaction(SIGINT, &old_int, NULL);
}

void	preprocess_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*cur;

	cur = cmds;
	shell->in_heredoc = 1;
	while (cur)
	{
		if (cur->redir.heredoc_fd == -2 && (cur->redir.heredocs
				|| cur->redir.heredoc_delim))
			handle_heredoc(cur, shell);
		cur = cur->next;
	}
	shell->in_heredoc = 0;
}
