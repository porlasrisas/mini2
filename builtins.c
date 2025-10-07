/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/10/07 16:40:57 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/* cd functions moved to builtins_cd.c */

int	builtin_exit(char **argv, int last_status)
{
	int	code;
	int	i;

	code = last_status;
	if (argv[1])
	{
		if (argv[2])
			return (ms_error2("exit", "too many arguments"), 1);
		i = 0;
		if (argv[1][i] == '+' || argv[1][i] == '-')
			i++;
		if (!argv[1][i])
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			return (255);
		}
		while (argv[1][i])
		{
			if (!ft_isdigit((unsigned char)argv[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(argv[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				return (255);
			}
			i++;
		}
		code = (unsigned char)ft_atoi(argv[1]);
	}
	return (code);
}
