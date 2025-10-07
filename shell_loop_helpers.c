/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 17:40:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <unistd.h>

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

static int	handle_blank_input(t_loop_ctx *c)
{
	if (is_blank_str(c->input) && isatty(STDIN_FILENO))
	{
		free(c->input);
		c->input = NULL;
		return (1);
	}
	return (0);
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

