/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:35:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/01 20:13:55 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_heredocs(t_hdoc *h)
{
	t_hdoc	*nx;

	while (h)
	{
		nx = h->next;
		if (h->delim)
			free(h->delim);
		free(h);
		h = nx;
	}
}

static void	close_redir_fds(t_redir *r)
{
	if (!r)
		return ;
	if (r->in_fd >= 0)
		close(r->in_fd);
	if (r->out_fd >= 0)
		close(r->out_fd);
	if (r->append_fd >= 0)
		close(r->append_fd);
	if (r->heredoc_fd >= 0)
		close(r->heredoc_fd);
}

static void	free_cmd_node(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	if (cmd->redir.heredoc_delim)
		free(cmd->redir.heredoc_delim);
	if (cmd->redir.heredocs)
		free_heredocs(cmd->redir.heredocs);
	close_redir_fds(&cmd->redir);
	free(cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;

	while (cmds)
	{
		next = cmds->next;
		free_cmd_node(cmds);
		cmds = next;
	}
}
