/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:33:14 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/27 10:16:55 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	fork_error(void)
{
	char	*msg;
	int		exit_code;

	msg = strerror(errno);
	ft_putstr_fd("minishell: fork error: ", 2);
	ft_putendl_fd(msg, 2);
	g_exit_status = 1;
	if (errno == EAGAIN)
		exit_code = 11;
	else
		exit_code = 1;
	return (exit_code);
}
