/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:05:36 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/13 11:33:03 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_env_name(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(*name))
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	free_env_var(char **env, int index)
{
	int	i;

	if (!env[index])
		return ;
	free(env[index]);
	i = index;
	while (env[i + 1])
	{
		env[i] = env[i + 1];
		i++;
	}
	env[i] = NULL;
}

static int	get_env_index(char **env, char *name)
{
	int		i;
	char	*equal_pos;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(name);
	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos && (size_t)(equal_pos - env[i]) == name_len
			&& !ft_strncmp(env[i], name, name_len))
			return (i);
		else if (!equal_pos && ft_strlen(env[i]) == name_len
			&& !ft_strncmp(env[i], name, name_len))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_unset(t_exec *exec, char **args)
{
	int	i;
	int	index;
	int	status;

	if (!args[1])
		return (0);
	status = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
		{
			index = get_env_index(exec->env, args[i]);
			if (index != -1)
				free_env_var(exec->env, index);
		}
	}
	return (status);
}
