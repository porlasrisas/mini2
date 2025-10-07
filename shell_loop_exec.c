/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 17:41:02 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <unistd.h>

int	is_blank_str(const char *s)
{
	int	i;

	i = 0;
	while (s && (s[i] == ' ' || s[i] == '\t'))
		i++;
	return (s && s[i] == '\0');
}

int	should_skip_single(t_cmd *cmd)
{
	return (cmd->skip_execution || (cmd->redir.has_redir_in
			&& cmd->redir.in_fd == -1));
}

void	exec_after_parsing(t_shell *shell, t_loop_ctx *c)
{
	if (!shell->parser.cmds->next)
	{
		if (shell->parser.cmds->is_builtin)
		{
			if (should_skip_single(shell->parser.cmds))
				shell->last_status = 1;
			else
				shell->last_status = run_single_builtin(shell,
						shell->parser.cmds, &c->save_in, &c->save_out);
		}
		else if (should_skip_single(shell->parser.cmds))
			shell->last_status = 1;
		else
			shell->last_status = execute_cmds(shell->parser.cmds, shell);
	}
	else
		shell->last_status = execute_cmds(shell->parser.cmds, shell);
}

void	process_line(t_shell *shell, t_loop_ctx *c)
{
	if (!c->input)
		return ;
	shell->parser.cmds = parse_input(c->input, shell->envp, shell->last_status);
	if (!shell->parser.cmds)
		return (free(c->input), (void)0);
	if (!shell->parser.cmds->next && shell->parser.cmds->redir.has_redir_in
		&& shell->parser.cmds->redir.in_fd == -1)
		shell->last_status = 1;
	preprocess_heredocs(shell->parser.cmds, shell);
	if (g_signal == SIGINT)
	{
		shell->last_status = 130;
		g_signal = 0;
		free_cmds(shell->parser.cmds);
		shell->parser.cmds = NULL;
		free(c->input);
		c->input = NULL;
		return ;
	}
	exec_after_parsing(shell, c);
	free_cmds(shell->parser.cmds);
	shell->parser.cmds = NULL;
	free(c->input);
}
