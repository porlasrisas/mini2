/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:48:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 16:52:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	is_builtin_name(const char *s)
{
	if (!s)
		return (0);
	if (ft_strncmp(s, "echo", 4) == 0 && s[4] == '\0')
		return (1);
	if (ft_strncmp(s, "cd", 2) == 0 && s[2] == '\0')
		return (1);
	if (ft_strncmp(s, "pwd", 3) == 0 && s[3] == '\0')
		return (1);
	if (ft_strncmp(s, "export", 6) == 0 && s[6] == '\0')
		return (1);
	if (ft_strncmp(s, "unset", 5) == 0 && s[5] == '\0')
		return (1);
	if (ft_strncmp(s, "env", 3) == 0 && s[3] == '\0')
		return (1);
	if (ft_strncmp(s, "exit", 4) == 0 && s[4] == '\0')
		return (1);
	return (0);
}

static char	**dup_argv(char **argv)
{
	int		count;
	int		k;
	char	**dup;

	count = 0;
	while (argv[count])
		count++;
	dup = (char **)malloc(sizeof(char *) * (count + 1));
	if (!dup)
		return (NULL);
	k = 0;
	while (k < count)
	{
		dup[k] = ft_strdup(argv[k]);
		k++;
	}
	dup[k] = NULL;
	return (dup);
}

static void	init_cmd_defaults(t_cmd *new)
{
	new->redir.in_fd = -1;
	new->redir.out_fd = -1;
	new->redir.append_fd = -1;
	new->redir.heredoc_fd = -1;
	new->redir.heredoc_delim = NULL;
	new->redir.heredoc_no_expand = 0;
	new->redir.has_redir_in = 0;
	new->redir.heredocs = NULL;
	new->is_builtin = 0;
	new->skip_execution = 0;
	new->next = NULL;
}

t_cmd	*add_cmd(t_cmd *cmds, char **argv)
{
	t_cmd	*new;
	t_cmd	*tmp;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = dup_argv(argv);
	if (!new->argv)
		return (free(new), NULL);
	init_cmd_defaults(new);
	if (cmds)
	{
		tmp = cmds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		cmds = new;
	if (new->argv &&new->argv[0] && is_builtin_name(new->argv[0]))
		new->is_builtin = 1;
	return (cmds);
}
