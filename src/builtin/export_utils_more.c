/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_more.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:05:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 13:23:28 by trouilla         ###   ########.fr       */
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

char	*get_var_value(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
		i++;
	if (!var[i])
		return (ft_strdup(""));
	else if (var[i] == '+')
		return (ft_strdup(var + i + 2));
	else
		return (ft_strdup(var + i + 1));
}

char	*append_values(char *name, char *old_value, char *new_value)
{
	char	*result;
	size_t	total_len;

	total_len = ft_strlen(name) + 1 + ft_strlen(old_value)
		+ ft_strlen(new_value) + 1;
	result = malloc(total_len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, name, total_len);
	ft_strlcat(result, "=", total_len);
	ft_strlcat(result, old_value, total_len);
	ft_strlcat(result, new_value, total_len);
	return (result);
}
