/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/08 18:30:30 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	int	status;

	(void)argc;
	(void)argv;
	status = shell_loop(envp);
	return (status);
}

void	dup_io_for_child(t_exec_ctx *s)
{
	if (s->i > 0)
	{
		if (dup2(s->pfd[s->i - 1][0], STDIN_FILENO) == -1)
			_exit(1);
	}
	if (s->i < s->n - 1)
	{
		if (dup2(s->pfd[s->i][1], STDOUT_FILENO) == -1)
			_exit(1);
	}
	close_all_pipes(s->n, s->pfd);
}

void	close_unused_fds(t_exec_ctx *s)
{
	if (s->n > 1)
	{
		if (s->i > 0)
		{
			if (s->pfd[s->i - 1][0] >= 0)
			{
				close(s->pfd[s->i - 1][0]);
				s->pfd[s->i - 1][0] = -1;
			}
		}
		if (s->i < s->n - 1)
		{
			if (s->pfd[s->i][1] >= 0)
			{
				close(s->pfd[s->i][1]);
				s->pfd[s->i][1] = -1;
			}
		}
	}
}
