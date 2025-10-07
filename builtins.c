/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 18:53:32 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* cd functions moved to builtins_cd.c */

static int	validate_exit_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	while (arg[i])
	{
		if (!ft_isdigit((unsigned char)arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (2);
		}
		i++;
	}
	return (0);
}

int	builtin_exit(char **argv, int last_status)
{
	int	code;
	int	validation_result;

	code = last_status;
	if (argv[1])
	{
		if (argv[2])
			return (ms_error2("exit", "too many arguments"), 1);
		validation_result = validate_exit_arg(argv[1]);
		if (validation_result != 0)
			return (validation_result);
		code = (unsigned char)ft_atoi(argv[1]);
	}
	return (code);
}
