/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:12:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 17:27:49 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <signal.h>

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
	return (shell.last_status);
}
