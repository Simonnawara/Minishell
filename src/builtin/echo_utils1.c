/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:42:04 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 18:50:19 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(char *var_name, char **env)
{
	int	i;
	int	var_len;

	if (!var_name || !env)
		return (NULL);
	var_len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, var_len) == 0
			&& env[i][var_len] == '=')
			return (env[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

int	expand_var(char *str, int *i, char **env, t_expand_data *data)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_len = 0;
	while (str[*i + var_len] && (ft_isalnum(str[*i + var_len]) || str[*i
				+ var_len] == '_'))
		var_len++;
	var_name = ft_substr(str, *i, var_len);
	if (!var_name)
	{
		free(data->result);
		return (0);
	}
	var_value = get_env_value(var_name, env);
	if (var_value)
	{
		ft_strlcpy(data->result + data->j, var_value, data->total_len - data->j
			+ 1);
		data->j += ft_strlen(var_value);
	}
	free(var_name);
	*i += var_len;
	return (1);
}

char	*process_var(char *str, int *i, char **env, size_t *total_len)
{
	size_t	var_len;
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_len = 0;
	while (str[*i + var_len] && (ft_isalnum(str[*i + var_len]) || str[*i
				+ var_len] == '_'))
		var_len++;
	var_name = ft_substr(str, *i, var_len);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(var_name, env);
	if (var_value)
		*total_len += ft_strlen(var_value);
	free(var_name);
	*i += var_len;
	return (var_value);
}

int	calculate_total_len(char *str, char **env, size_t *total_len)
{
	int	i;

	i = 0;
	*total_len = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i
				+ 1] != '"' && str[i + 1] != '\'')
		{
			if (process_var(str, &i, env, total_len) == NULL)
				(*total_len) += 0;
		}
		else
		{
			(*total_len)++;
			i++;
		}
	}
	return (1);
}

char	*expand_variables(char *str, char **env)
{
	t_expand_data	data;
	int				i;

	if (!str || !*str)
		return (ft_strdup(""));
	if (!calculate_total_len(str, env, &data.total_len))
		return (NULL);
	data.result = malloc(sizeof(char) * (data.total_len + 1));
	if (!data.result)
		return (NULL);
	i = 0;
	data.j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i
				+ 1] != '"' && str[i + 1] != '\'')
		{
			if (!expand_var(str, &i, env, &data))
				return (NULL);
		}
		else
			data.result[data.j++] = str[i++];
	}
	data.result[data.j] = '\0';
	return (data.result);
}
