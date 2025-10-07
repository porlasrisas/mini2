/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 23:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 00:41:35 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_not_valid(const char *name)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	export_with_equal(char *name, char *eq, t_shell *shell)
{
	*eq = '\0';
	if (!is_valid_identifier(name))
	{
		print_not_valid(name);
		*eq = '=';
		return (1);
	}
	env_set(&shell->envp, name, eq + 1);
	*eq = '=';
	return (0);
}

static int	export_variable(char *arg, t_shell *shell)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		return (export_with_equal(arg, eq, shell));
	if (!is_valid_identifier(arg))
	{
		print_not_valid(arg);
		return (1);
	}
	env_set(&shell->envp, arg, "");
	return (0);
}

/* print_export_sorted moved to export_utils.c */

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_export_sorted(shell->envp);
		return (0);
	}
	while (argv[i])
	{
		if (export_variable(argv[i], shell) != 0)
			status = 1;
		i++;
	}
	return (status);
}
