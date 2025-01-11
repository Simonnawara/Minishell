/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:16:17 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/11 11:22:02 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*create_env_string(const char *key, const char *value)
{
	char	*new_var;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_var = (char *)malloc(key_len + value_len + 2);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, key, key_len + 1);
	new_var[key_len] = '=';
	ft_strlcpy(new_var + key_len + 1, value, value_len + 1);

	return (new_var);
}

static int	find_env_variable(char **env, const char *key)
{
	int		i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);  
}

static char	**add_env_variable(char **env, char *new_var)
{
	char	**new_env;
	int		i;
	int		size;

	size = 0;
	while (env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

int	set_env_value(char **env, const char *key, const char *value)
{
	char	*new_var;
	int		index;

	if (!env || !key || !value)
		return (-1);
	new_var = create_env_string(key, value);
	if (!new_var)
		return (-1);
	index = find_env_variable(env, key);
	if (index >= 0)
	{
		free(env[index]);
		env[index] = new_var;
	}
	else
	{
				env = add_env_variable(env, new_var);
		if (!env)
		{
			free(new_var);
			return (-1);
		}
	}
	return (0);
}
