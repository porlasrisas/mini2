/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_core_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:50:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 18:49:26 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	alloc_exec_arrays(t_exec_ctx *s)
{
	if (s->n > 1)
	{
		s->pfd = malloc(sizeof(int) * (s->n - 1));
		if (!s->pfd)
			return (ms_perror("malloc"), -1);
	}
	s->pids = malloc(sizeof(pid_t) * s->n);
	if (!s->pids)
	{
		free(s->pfd);
		return (ms_perror("malloc"), -1);
	}
	s->cmd_arr = malloc(sizeof(t_cmd *) * s->n);
	if (!s->cmd_arr)
	{
		free(s->pfd);
		free(s->pids);
		return (ms_perror("malloc"), -1);
	}
	return (0);
}

static void	fill_cmd_array(t_exec_ctx *s, t_cmd *cmds)
{
	s->cur = cmds;
	s->i = 0;
	while (s->i < s->n)
	{
		s->cmd_arr[s->i] = s->cur;
		s->cur = s->cur->next;
		s->i++;
	}
}

int	init_ctx(t_exec_ctx *s, t_cmd *cmds)
{
	s->n = count_cmds(cmds);
	if (s->n == 0)
		return (0);
	s->pfd = NULL;
	s->pids = NULL;
	s->cmd_arr = NULL;
	if (alloc_exec_arrays(s) != 0)
		return (-1);
	fill_cmd_array(s, cmds);
	return (1);
}
