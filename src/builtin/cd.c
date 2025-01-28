/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:56:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 12:50:12 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	update_env_var(t_exec *exec, const char *name, const char *value)
{
	char	*var;
	char	*args[3];
	int		ret;

	if (!exec || !exec->env || !name || !value)
		return (1);
	if (init_export_args(args, name, value, &var))
		return (1);
	ret = ft_export(args, exec);
	free(args[0]);
	free(args[1]);
	return (ret);
}

static int	handle_home_cd(t_exec *exec, char **target, char *pwd)
{
	*target = get_env_value1(exec->env, "HOME");
	if (!*target)
	{
		free(pwd);
		ft_putendl_fd("cd: HOME not set", 2);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

static int	handle_oldpwd_cd(t_exec *exec, char **target, char *pwd)
{
	*target = get_env_value1(exec->env, "OLDPWD");
	if (!*target)
	{
		free(pwd);
		ft_putendl_fd("cd: OLDPWD not set", 2);
		g_exit_status = 1;
		return (1);
	}
	ft_putendl_fd(*target, 1);
	return (0);
}

static int	get_cd_target(char **args, t_exec *exec, char **target, char *pwd)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		if (handle_home_cd(exec, target, pwd))
			return (1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (handle_oldpwd_cd(exec, target, pwd))
			return (1);
	}
	else
	{
		*target = get_absolute_path(pwd, args[1]);
		if (!*target)
		{
			free(pwd);
			return (1);
		}
	}
	return (0);
}

int	ft_cd(char **args, t_exec *exec)
{
	char	*target;
	char	*pwd;
	int		ret;

	if (!exec || !exec->env)
		return (1);
	pwd = get_env_value1(exec->env, "PWD");
	if (!pwd)
		pwd = ft_strdup("/");
	if (get_cd_target(args, exec, &target, pwd))
		return (1);
	ret = chdir(target);
	if (ret == 0)
		ret = update_pwd_vars(exec, target);
	else
	{
		print_error(args);
		ret = 1;
	}
	free(pwd);
	free(target);
	return (ret);
}
