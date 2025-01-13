/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:45:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/13 11:05:41 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_id(char *str)
{
	int	i;

	if (!str || !*str || ft_isdigit(*str))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	update_env(char **env, char *var)
{
	int		i;
	char	*key;
	int		key_len;

	i = 0;
	key_len = 0;
	while (var[key_len] && var[key_len] != '=')
		key_len++;
	key = ft_substr(var, 0, key_len);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && 
			(env[i][key_len] == '=' || env[i][key_len] == '\0'))
		{
			free(env[i]);
			env[i] = ft_strdup(var);
			free(key);
			return ;
		}
		i++;
	}
	free(key);
	env[i] = ft_strdup(var);
	env[i + 1] = NULL;
}

static char	**realloc_env(char **env)
{
	char	**new_env;
	int		size;
	int		i;

	size = 0;
	while (env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		free(env[i]);
		i++;
	}
	new_env[i] = NULL;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

static int	add_to_env(t_exec *exec, char *var)
{
	int		env_size;
	char	**new_env;

	env_size = 0;
	while (exec->env[env_size])
		env_size++;
	if (env_size % 10 == 0)
	{
		new_env = realloc_env(exec->env);
		if (!new_env)
			return (1);
		exec->env = new_env;
	}
	update_env(exec->env, var);
	return (0);
}

int	ft_export(char **args, t_exec *exec)
{
	int		i;
	int		status;

	if (!args[1])
	{
		i = 0;
		while (exec->env[i])
			ft_printf("declare -x %s\n", exec->env[i++]);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				args[i]);
			status = 1;
		}
		else if (add_to_env(exec, args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}