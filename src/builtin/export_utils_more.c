/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_more.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:05:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:02:03 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_id(char *str)
{
	int	i;
	int	equal_pos;

	if (!str || !*str || ft_isdigit(*str))
		return (0);
	i = 0;
	equal_pos = -1;
	while (str[i])
	{
		if (str[i] == '=' || (str[i] == '+' && str[i + 1] == '='))
		{
			equal_pos = i;
			break ;
			(void)equal_pos;
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*get_var_name(char *var)
{
	int		i;
	char	*name;

	i = 0;
	while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
		i++;
	name = ft_substr(var, 0, i);
	return (name);
}

static char	*handle_quotes(char *value)
{
	int		i;
	char	*trimmed_value;

	if (value[0] == '\'' || value[0] == '\"')
	{
		i = 0;
		while (value[i + 1])
			i++;
		if (value[i] == value[0])
		{
			trimmed_value = ft_substr(value, 1, i - 1);
			free(value);
			return (trimmed_value);
		}
	}
	return (value);
}

char	*get_var_value(char *var)
{
	int		i;
	char	*value;

	i = 0;
	while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
		i++;
	if (!var[i])
		return (ft_strdup(""));
	else if (var[i] == '+')
		value = ft_strdup(var + i + 2);
	else
		value = ft_strdup(var + i + 1);
	if (!value)
		return (NULL);
	return (handle_quotes(value));
}

char	*append_values(char *name, char *old_value, char *new_value)
{
	char	*result;
	size_t	total_len;
	char	*temp;

	if (!old_value)
		old_value = "";
	total_len = ft_strlen(name) + 1 + ft_strlen(old_value)
		+ ft_strlen(new_value) + 1;
	result = malloc(total_len);
	if (!result)
		return (NULL);
	temp = malloc(ft_strlen(old_value) + ft_strlen(new_value) + 1);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	ft_strlcpy(temp, old_value, ft_strlen(old_value) + 1);
	ft_strlcat(temp, new_value, ft_strlen(old_value) + ft_strlen(new_value)
		+ 1);
	ft_strlcpy(result, name, total_len);
	ft_strlcat(result, "=", total_len);
	ft_strlcat(result, temp, total_len);
	free(temp);
	return (result);
}
