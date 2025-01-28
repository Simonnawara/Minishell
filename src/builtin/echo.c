/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 18:44:34 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_echo_arg(char **res, int res_index, t_exec *exec,
		int *prev_was_quote)
{
	char	*stripped_arg;
	int		ret;

	stripped_arg = strip_quotes(res[res_index]);
	if (!stripped_arg)
		return (1);
	if (ft_strchr(stripped_arg, '$'))
	{
		ret = process_dollar_arg(stripped_arg, res[res_index], exec, NULL);
		if (ret > 0)
			return (ret - 1);
	}
	else
		handle_stripped_arg(stripped_arg);
	if (check_space_conditions(res, res_index, prev_was_quote))
		ft_putstr_fd(" ", 1);
	return (0);
}

int	is_valid_n_option(const char *str)
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

int	handle_echo_args(char **res, int res_index, t_exec *exec,
		int *prev_was_quote)
{
	while (res[res_index] && res[res_index][0] != '>'
		&& res[res_index][0] != '<' && res[res_index][0] != '|')
	{
		if (process_echo_arg(res, res_index, exec, prev_was_quote))
			return (1);
		res_index++;
	}
	return (0);
}

int	check_n_option(char **args, int *i, int *n_option)
{
	*n_option = 0;
	while (args[*i] && is_valid_n_option(args[*i]))
	{
		*n_option = 1;
		(*i)++;
	}
	return (*i);
}

int	ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
{
	int	i;
	int	n_option;
	int	res_index;
	int	prev_was_quote;

	if (!args || !res)
		return (1);
	i = 1;
	res_index = echo_counter + i;
	prev_was_quote = 0;
	res_index = echo_counter + check_n_option(args, &i, &n_option);
	if (handle_echo_args(res, res_index, exec, &prev_was_quote))
		return (1);
	if (!n_option)
		write(1, "\n", 1);
	return (0);
}
