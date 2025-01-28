/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:04:00 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 13:21:27 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_env_var(char **env, char *name)
{
	int		i;
	size_t	name_len;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && (env[i][name_len] == '='
			|| env[i][name_len] == '\0'))
			return (env[i]);
		i++;
	}
	return (NULL);
}

int	is_append_operation(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
		i++;
	return (var[i] == '+' && var[i + 1] == '=');
}

char	*create_new_value(char *name, char *old_value, char *new_value,
		int is_append)
{
	char	*result;
	size_t	total_len;

	if (is_append && old_value)
		return (append_values(name, old_value, new_value));
	total_len = ft_strlen(name) + 1 + ft_strlen(new_value) + 1;
	result = malloc(total_len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, name, total_len);
	ft_strlcat(result, "=", total_len);
	ft_strlcat(result, new_value, total_len);
	return (result);
}

char	*get_existing_value(char **env, char *name, int is_append)
{
	char	*old_value;
	char	*temp;

	old_value = NULL;
	if (is_append)
	{
		temp = ft_strchr(find_env_var(env, name), '=');
		if (temp)
			old_value = temp + 1;
		else
			old_value = "";
	}
	return (old_value);
}

int	update_existing_var(char **env, char *name, char *new_value, int is_append)
{
	char	*old_value;
	char	*final_value;
	int		i;

	old_value = get_existing_value(env, name, is_append);
	final_value = create_new_value(name, old_value, new_value, is_append);
	if (!final_value)
		return (1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& (env[i][ft_strlen(name)] == '='
			|| env[i][ft_strlen(name)] == '\0'))
		{
			free(env[i]);
			env[i] = final_value;
			return (0);
		}
		i++;
	}
	return (0);
}
