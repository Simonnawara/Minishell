/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:16:17 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/13 14:40:51 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_env_index(char **env, char *key)
{
	int	i;
	int	key_len;

	if (!key || !env)
		return (-1);
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

static char	*create_env_string(char *key, char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}

int	update_env_variable(char **env, char *key, char *value)
{
	int		index;
	char	*new_var;

	if (!env || !key || !value)
		return (1);
	new_var = create_env_string(key, value);
	if (!new_var)
	{
		free(value);
		return (1);
	}
	index = find_env_index(env, key);
	if (index != -1)
	{
		free(env[index]);
		env[index] = new_var;
		free(value);
		return (0);
	}
	free(new_var);
	free(value);
	return (1);
}