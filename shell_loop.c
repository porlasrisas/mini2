/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:30:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/10/08 19:07:18 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	shell_loop(char **envp)
{
	t_shell				shell;
	struct sigaction	sa_pipe;
	t_loop_ctx			c;

	init_shell_state(&shell, &sa_pipe, &c, envp);
	while (1)
	{
		c.input = read_input(&shell);
		if (!handle_input_result(&shell, &c))
			break ;
		process_line(&shell, &c);
	}
	restore_terminal();
	free_env(shell.envp);
	return (shell.last_status);
}

static int	handle_sigint_primary(t_shell *shell, t_loop_ctx *c)
{
	if (g_signal == SIGINT)
	{
		shell->last_status = 1;
		g_signal = 0;
		free(c->input);
		c->input = NULL;
		return (1);
	}
	return (0);
}

static int	handle_eof_no_tty(t_shell *shell, t_loop_ctx *c)
{
	if (!c->input)
	{
		if (isatty(STDIN_FILENO))
		{
			shell->last_status = 0;
			write(STDOUT_FILENO, "exit\n", 5);
		}
		return (1);
	}
	return (0);
}

static void	handle_sigint_secondary(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->last_status = 130;
		g_signal = 0;
	}
}

int	handle_input_result(t_shell *shell, t_loop_ctx *c)
{
	if (handle_sigint_primary(shell, c))
		return (1);
	if (handle_eof_no_tty(shell, c))
		return (0);
	handle_sigint_secondary(shell);
	if (handle_blank_input(c))
		return (1);
	return (1);
}
