/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:38:34 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 12:51:00 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_error(char **args)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(args[1], 2);
	g_exit_status = 1;
}

char	*get_env_value1(char **env, const char *var)
{
	int		i;
	int		var_len;
	char	*value;

	if (!env || !var)
		return (NULL);
	var_len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
		{
			value = ft_strdup(env[i] + var_len + 1);
			return (value);
		}
		i++;
	}
	return (NULL);
}

int	update_pwd_vars(t_exec *exec, const char *new_path)
{
	char	*old_pwd;
	int		ret;

	if (!exec || !new_path)
		return (1);
	old_pwd = get_env_value1(exec->env, "PWD");
	if (old_pwd)
	{
		ret = update_env_var(exec, "OLDPWD", old_pwd);
		free(old_pwd);
		if (ret != 0)
			return (ret);
	}
	return (update_env_var(exec, "PWD", new_path));
}

char	*get_absolute_path(char *base, char *arg)
{
	char	*result;
	char	*temp;

	if (!arg)
		return (NULL);
	if (arg[0] == '/')
	{
		result = ft_strdup(arg);
		return (result);
	}
	if (base[ft_strlen(base) - 1] == '/')
		result = ft_strjoin(base, arg);
	else
	{
		temp = ft_strjoin(base, "/");
		if (!temp)
			return (NULL);
		result = ft_strjoin(temp, arg);
		free(temp);
	}
	return (result);
}

int	init_export_args(char **args, const char *name,
		const char *value, char **var)
{
	*var = ft_strjoin(name, "=");
	if (!(*var))
		return (1);
	args[0] = ft_strdup("export");
	if (!args[0])
	{
		free(*var);
		return (1);
	}
	args[1] = ft_strjoin(*var, value);
	free(*var);
	if (!args[1])
	{
		free(args[0]);
		return (1);
	}
	args[2] = NULL;
	return (0);
}
