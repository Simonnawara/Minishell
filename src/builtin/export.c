/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:45:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/27 10:04:08 by trouilla         ###   ########.fr       */
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
static int	is_heap_pointer(void *ptr)
{
	unsigned long	addr;

	addr = (unsigned long)ptr;
	return (addr < 0x7fff00000000UL && addr > 0x1000UL);
}

static int	update_env(char **env, char *var)
{
	int		i;
	char	*key;
	int		key_len;
	char	*new_value;
	char	*old_value;

	if (!env || !var)
		return (1);
	i = 0;
	key_len = 0;
	while (var[key_len] && var[key_len] != '=')
		key_len++;
	key = ft_substr(var, 0, key_len);
	if (!key)
		return (1);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && (env[i][key_len] == '='
				|| env[i][key_len] == '\0'))
		{
			old_value = env[i];
			new_value = ft_strdup(var);
			if (!new_value)
			{
				free(key);
				return (1);
			}
			env[i] = new_value;
			if (is_heap_pointer(old_value))
				free(old_value);
			free(key);
			return (0);
		}
		i++;
	}
	new_value = ft_strdup(var);
	if (!new_value)
	{
		free(key);
		return (1);
	}
	free(key);
	env[i] = new_value;
	env[i + 1] = NULL;
	return (0);
}
static char	**realloc_env(char **env)
{
	char	**new_env;
	int		size;
	int		i;

	size = 0;
	while (env[size])
	{
		size++;
	}
	if (!(new_env = (char **)malloc(sizeof(char *) * (size + 2))))
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (!(new_env[i] = ft_strdup(env[i])))
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
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

	if (!exec || !exec->env)
		return (1);
	env_size = 0;
	while (exec->env[env_size])
		env_size++;
	if (env_size % 10 == 9)
	{
		new_env = realloc_env(exec->env);
		if (!new_env)
			return (1);
		exec->env = new_env;
	}
	return (update_env(exec->env, var));
}

int	ft_export(char **args, t_exec *exec)
{
	int i;
	int status;

	if (!exec || !exec->env || !args)
		return (1);

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
			status = 1;
		else if (add_to_env(exec, args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}