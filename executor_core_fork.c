/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_core_fork.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 14:42:48 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	dup_io_for_child(t_exec_ctx *s)
{
	if (s->n > 1)
	{
		if (s->i > 0)
			dup2(s->pfd[s->i - 1][0], STDIN_FILENO);
		if (s->i < s->n - 1)
			dup2(s->pfd[s->i][1], STDOUT_FILENO);
		s->j = 0;
		while (s->j < s->n - 1)
		{
			close(s->pfd[s->j][0]);
			close(s->pfd[s->j][1]);
			s->j++;
		}
	}
}

static void	close_unused_fds(t_exec_ctx *s)
{
	if (s->n > 1)
	{
		if (s->i > 0)
			close(s->pfd[s->i - 1][0]);
		if (s->i < s->n - 1)
			close(s->pfd[s->i][1]);
	}
}

void	fork_children(t_exec_ctx *s, t_shell *shell)
{
	s->ign.sa_handler = SIG_IGN;
	sigemptyset(&s->ign.sa_mask);
	s->ign.sa_flags = 0;
	sigaction(SIGINT, &s->ign, &s->old_int);
	s->i = s->n - 1;
	while (s->i >= 0)
	{
		s->node = s->cmd_arr[s->i];
		s->pids[s->i] = fork();
		if (s->pids[s->i] == -1)
			s->pids[s->i] = -1;
		else if (s->pids[s->i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGPIPE, SIG_DFL);
			dup_io_for_child(s);
			exec_command_child(s->node, shell, (s->n > 1));
			_exit(1);
		}
		else
			close_unused_fds(s);
		s->i--;
	}
}
