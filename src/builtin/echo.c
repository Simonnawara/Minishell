/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 16:08:52 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*strip_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

static void	cleanup_echo_resources(char *stripped_arg, char *processed_arg,
		char *expanded_arg)
{
	if (stripped_arg)
		free(stripped_arg);
	if (processed_arg)
		free(processed_arg);
	if (expanded_arg)
		free(expanded_arg);
}

static void	handle_expanded_arg(char *expanded_arg, char *stripped_arg,
		char *processed_arg)
{
	ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
	cleanup_echo_resources(NULL, processed_arg, expanded_arg);
	free(stripped_arg);
}

static void	handle_processed_arg(char *processed_arg, char *stripped_arg)
{
	(void)stripped_arg;
	ft_putstr_fd(processed_arg, 1);
	cleanup_echo_resources(NULL, processed_arg, NULL);
}

static void	handle_stripped_arg(char *stripped_arg)
{
	ft_putstr_fd(stripped_arg, 1);
	free(stripped_arg);
}

static int	process_dollar_arg(char *stripped_arg, char *res_i, t_exec *exec,
		char *processed_arg)
{
	char	quote_type;
	int		total_quotes;
	char	*expanded_arg;

	quote_type = 0;
	if (res_i[0] == res_i[ft_strlen(res_i) - 1] && (res_i[0] == 34
			|| res_i[0] == 39))
		quote_type = res_i[0];
	total_quotes = count_quotes(res_i, quote_type);
	processed_arg = get_command(res_i, total_quotes, quote_type);
	if (!processed_arg)
	{
		free(stripped_arg);
		return (1);
	}
	if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg, '$'))
	{
		if (check_exit_status(processed_arg))
		{
			expanded_arg = check_and_replace_exit_status(processed_arg,
					g_exit_status);
			printf("%s\n", expanded_arg);
			cleanup_echo_resources(stripped_arg, processed_arg, expanded_arg);
			return (2);
		}
		expanded_arg = expand_variables(processed_arg, exec->env);
		if (!expanded_arg)
		{
			cleanup_echo_resources(stripped_arg, processed_arg, NULL);
			return (1);
		}
		handle_expanded_arg(expanded_arg, stripped_arg, processed_arg);
	}
	else
		handle_processed_arg(processed_arg, stripped_arg);
	return (0);
}

static int	process_echo_arg(char **res, int res_index, t_exec *exec,
		int *prev_was_quote)
{
	char	*stripped_arg;
	char	*processed_arg;
	int		next_is_quote;
	int		curr_is_quote;
	int		ret;

	stripped_arg = strip_quotes(res[res_index]);
	if (!stripped_arg)
		return (1);
	processed_arg = NULL;
	if (ft_strchr(stripped_arg, '$'))
	{
		ret = process_dollar_arg(stripped_arg, res[res_index], exec,
				processed_arg);
		if (ret > 0)
			return (ret - 1);
	}
	else
		handle_stripped_arg(stripped_arg);
	next_is_quote = (res[res_index + 1] && (res[res_index + 1][0] == '"'
				|| res[res_index + 1][0] == '\''));
	curr_is_quote = (res[res_index][0] == '"' || res[res_index][0] == '\'');
	if (res[res_index + 1] && res[res_index + 1][0] != '>' && res[res_index
		+ 1][0] != '<' && !*prev_was_quote && !next_is_quote && !curr_is_quote)
		ft_putstr_fd(" ", 1);
	*prev_was_quote = curr_is_quote || next_is_quote;
	return (0);
}

static int	is_valid_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

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

static char	*process_var(char *str, int *i, char **env, size_t *total_len)
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

static int	calculate_total_len(char *str, char **env, size_t *total_len)
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

static int	expand_var(char *str, int *i, char **env, t_expand_data *data)
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

int	ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
{
	int i;
	int n_option;
	int res_index;
	int prev_was_quote;

	if (!args || !res)
		return (1);
	i = 1;
	res_index = echo_counter + i;
	n_option = 0;
	prev_was_quote = 0;
	while (args[i] && is_valid_n_option(args[i]))
	{
		n_option = 1;
		i++;
		res_index = echo_counter + i;
	}
	while (res[res_index] && res[res_index][0] != '>'
		&& res[res_index][0] != '<' && res[res_index][0] != '|')
	{
		if (process_echo_arg(res, res_index, exec, &prev_was_quote))
			return (1);
		i++;
		res_index = echo_counter + i;
	}
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}
