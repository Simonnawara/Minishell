/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 09:25:21 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/11 10:27:10 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *get_envir_value(char**env, char *key)
{
	int key_len;
	int i;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if(ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static int home_handle_directory(char **env)
{
	char *home_path;

	home_path = get_envir_value(env, "HOME");
	if (!home_path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	if (chdir(home_path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(home_path);
		return (1);
	}
	return (0);
}
static void uptade_pwd_vars(char **env)
{
	char *old_pwd;
	//char *current_pwd;
	char buffer[PATH_MAX];

	old_pwd = get_envir_value(env, "PWD");
	if (old_pwd)
	{
		set_env_value(env, "OLDPWD", old_pwd);
	}
	if (getcwd(buffer, PATH_MAX))
	{
		set_env_value(env, "PWD", buffer);
	}
}

int ft_cd(char **args, char **env)
{
	char *path;

	if (!args[1])
		return (home_handle_directory(env));
	if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_envir_value(env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (1);
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	uptade_pwd_vars(env);
	return (0);
}