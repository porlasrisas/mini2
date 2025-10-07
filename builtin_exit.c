/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 23:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/30 19:32:18 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	print_non_numeric(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	return (255);
}

int	builtin_exit(char **argv, int last_status)
{
	int	code;

	code = last_status;
	if (argv[1])
	{
		if (argv[2])
		{
			ms_error2("exit", "too many arguments");
			return (1);
		}
		if (!is_numeric(argv[1]))
			return (print_non_numeric(argv[1]));
		code = (unsigned char)ft_atoi(argv[1]);
	}
	return (code);
}
