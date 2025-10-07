/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:18:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 17:13:01 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

static char	*read_input_tty(t_shell *shell)
{
	char	*line;

	line = readline("minishell$ ");
	(void)shell;
	if (line && *line && !shell->in_heredoc)
		add_history(line);
	return (line);
}

static char	*read_input_non_tty(void)
{
	static int	printed;

	if (!printed)
	{
		write(STDOUT_FILENO, "minishell$\n", 11);
		printed = 1;
	}
	return (ms_read_line_fd(STDIN_FILENO));
}

char	*read_input(t_shell *shell)
{
	if (isatty(STDIN_FILENO))
		return (read_input_tty(shell));
	return (read_input_non_tty());
}
