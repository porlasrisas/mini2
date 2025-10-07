/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 15:07:34 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <signal.h>
#include <unistd.h>

static void	init_shell_defaults(t_shell *shell)
{
	shell->in_heredoc = 0;
	shell->parser.input = NULL;
	shell->parser.cmds = NULL;
	shell->parser.pipe_count = 0;
	shell->last_status = 0;
}

void	init_shell_state(t_shell *shell, struct sigaction *sa_pipe,
		t_loop_ctx *c, char **envp)
{
	c->save_in = -1;
	c->save_out = -1;
	shell->envp = env_dup(envp);
	init_shell_defaults(shell);
	sa_pipe->sa_handler = SIG_IGN;
	sigemptyset(&sa_pipe->sa_mask);
	sa_pipe->sa_flags = 0;
	sigaction(SIGPIPE, sa_pipe, NULL);
	setup_signals();
	configure_terminal();
}

/* moved helpers to shell_loop_helpers.c to keep functions short */
