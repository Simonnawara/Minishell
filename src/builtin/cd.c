/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:09:28 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/13 15:12:48 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_cd_error(char **args)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (args[2])
		ft_putstr_fd("string not in pwd: ", 2);
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(args[1], 2);
}

static char	*get_env_var(char **env, const char *var)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}
static int	set_oldpwd(char **env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	int		i;

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (1);
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	if (!oldpwd)
		return (1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			free(env[i]);
			env[i] = oldpwd;
			return (0);
		}
		i++;
	}
	free(oldpwd);
	return (1);
}

static int	handle_home_path(t_exec *exec)
{
	char	*home;
	int		ret;

	home = get_env_var(exec->env, "HOME");
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	ret = 0;
	if (set_oldpwd(exec->env) != 0)
		ret = 1;
	if (ret == 0 && chdir(home) != 0)
		ret = 1;
	free(home);
	return (ret);
}
static int	handle_oldpwd(t_exec *exec)
{
	char	*oldpwd;
	int		ret;

	oldpwd = get_env_var(exec->env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (1);
	}
	ret = 0;
	if (set_oldpwd(exec->env) != 0)
		ret = 1;
	if (ret == 0 && chdir(oldpwd) != 0)
		ret = 1;
	free(oldpwd);
	return (ret);
}

static int	change_dir(char *path, t_exec *exec)
{
	if (set_oldpwd(exec->env) != 0)
		return (1);
	if (chdir(path) != 0)
		return (1);
	return (0);
}

int	ft_cd(t_exec *exec, char **args)
{
	int	ret;

	if (!args[1])
		return (handle_home_path(exec));
	if (ft_strcmp(args[1], "-") == 0)
		return (handle_oldpwd(exec));
	ret = change_dir(args[1], exec);
	if (ret != 0)
		print_cd_error(args);
	return (ret);
}
