/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:05:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/29 10:51:50 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ms_perror(const char *subject)
{
	ft_putstr_fd("minishell: ", 2);
	if (subject)
	{
		ft_putstr_fd((char *)subject, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	ms_error2(const char *subject, const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (subject)
	{
		ft_putstr_fd((char *)subject, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
}

void	ms_syntax_error(const char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)tok, 2);
	ft_putstr_fd("'\n", 2);
}
