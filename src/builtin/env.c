/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:19:51 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/11 12:31:03 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	print_env_error(void)
{
	ft_putstr_fd("env: too many arguments\n", 2);
	return (1);
}

int	ft_env(t_exec *exec, char **args)
{
	int	i;

	if (!exec || !exec->env)
		return (1);
	if (args && args[1])
		return (print_env_error());
	i = 0;
	while (exec->env[i])
	{
		ft_putendl_fd(exec->env[i], 1);
		i++;
	}
	return (0);
}
