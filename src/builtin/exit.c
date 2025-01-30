/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:43:51 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:00:08 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_str_isdigit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_exit_arg(char *arg)
{
	long long	num;

	if (!ft_str_isdigit(arg))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (-1);
	}
	num = ft_atoi(arg);
	if (num > 255 || num < 0)
		num = num % 256;
	if (num < 0)
		num += 256;
	return (num);
}

static void	free_executor(t_exec *exec)
{
	int	i;

	if (exec->ast)
		free_ast(exec->ast);
	if (exec->path_info)
	{
		if (exec->path_info->paths)
			free_array(exec->path_info->paths);
		free(exec->path_info->path_var);
		free(exec->path_info->full_path);
		free(exec->path_info->temp);
		free(exec->path_info);
	}
	if (exec->env)
	{
		i = 0;
		while (exec->env[i])
		{
			if (exec->env_allocated && exec->env_allocated[i])
				free(exec->env[i]);
			i++;
		}
		free(exec->env);
	}
	free(exec->env_allocated);
}

int	ft_exit(t_ast_node *node, t_exec *exec)
{
	int	exit_code;

	ft_putendl_fd("exit", 1);
	if (!node->args || !node->args[1])
	{
		exit_code = exec->last_status;
		free_executor(exec);
		exit(exit_code);
	}
	if (node->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit_code = check_exit_arg(node->args[1]);
	if (exit_code == -1)
	{
		free_executor(exec);
		exit(255);
	}
	free_executor(exec);
	exit(exit_code);
}
