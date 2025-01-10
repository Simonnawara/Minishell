/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:37:07 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 11:41:45 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_present(char *arg)
{
	int i;
	
	if(!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (!arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static int print_args(char **args, int index, int n_option)
{
	int i;

	i = index;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_option)
		ft_putchar_fd('\n', 1);
}
int ft_echo(char **args)
{
	int i;
	int n_option;

	if (!args || !args[1])
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	i = 1;
	n_option = 0;
	while (args[i] && is_n_present(args[i]))
	{
		n_option = 1;
		i++;
	}
	print_args(args, i, n_option);
	return (0);
}