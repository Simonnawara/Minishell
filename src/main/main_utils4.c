/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:28:55 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 10:32:13 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_exit_status(char *expanded_arg)
{
	printf("%s : command not found\n", expanded_arg);
	g_exit_status = 127;
	return (1);
}

char	*expand_exit_status(char *processed_arg, int *stop_processing)
{
	char	*expanded_arg;

	if (check_exit_status(processed_arg))
	{
		expanded_arg = check_and_replace_exit_status(processed_arg,
				g_exit_status);
		return (expanded_arg);
	}
	*stop_processing = 0;
	return (NULL);
}

int	handle_expansion(char **cmd, char **processed_arg, char **env, int i)
{
	char	*expanded_arg;
	int		stop_processing;

	stop_processing = 1;
	expanded_arg = expand_exit_status(*processed_arg, &stop_processing);
	if (stop_processing && i == 0)
	{
		handle_exit_status(expanded_arg);
		return (1);
	}
	if (expanded_arg)
	{
		*cmd = expanded_arg;
		return (0);
	}
	expanded_arg = expand_variables(*processed_arg, env);
	if (expanded_arg)
		*cmd = expanded_arg;
	return (0);
}

int	handle_home_expansion(char **cmd, char **res, int i, char **env)
{
	char	*home_value;
	char	*full_path;

	home_value = init_home_value(env);
	if (!home_value)
		return (1);
	if (strlen(res[i]) > 1)
		full_path = ft_strjoin(home_value, res[i] + 1);
	else
		full_path = ft_strdup(home_value);
	if (!full_path)
	{
		if (!get_env_value("HOME", env))
			free(home_value);
		return (1);
	}
	free(res[i]);
	res[i] = full_path;
	*cmd = ft_strdup(full_path);
	if (handle_empty_cmd(cmd, home_value, full_path, env))
		return (1);
	if (!get_env_value("HOME", env))
		free(home_value);
	return (0);
}

t_quote_data	*init_quote_data(char **res, int i)
{
	t_quote_data	*data;

	data = malloc(sizeof(t_quote_data));
	if (!data)
		return (NULL);
	data->res = res;
	data->i = i;
	return (data);
}
