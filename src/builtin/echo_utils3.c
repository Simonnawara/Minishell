/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:43:46 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 11:12:45 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_exit_status_check(char *stripped_arg, char *processed_arg,
		char *expanded_arg)
{
	printf("%s\n", expanded_arg);
	cleanup_echo_resources(stripped_arg, processed_arg, expanded_arg);
	return (2);
}

int	handle_dollar_expansion(char *stripped_arg, char *processed_arg,
		t_exec *exec)
{
	char	*expanded_arg;

	expanded_arg = expand_variables(processed_arg, exec->env);
	if (!expanded_arg)
	{
		cleanup_echo_resources(stripped_arg, processed_arg, NULL);
		return (1);
	}
	handle_expanded_arg(expanded_arg, stripped_arg, processed_arg);
	return (0);
}

int	process_quoted_dollar(char *stripped_arg, char *processed_arg,
		t_exec *exec)
{
	char	*expanded_arg;

	if (check_exit_status(processed_arg))
	{
		expanded_arg = check_and_replace_exit_status(processed_arg,
				g_exit_status);
		return (handle_exit_status_check(stripped_arg, processed_arg,
				expanded_arg));
	}
	return (handle_dollar_expansion(stripped_arg, processed_arg, exec));
}

int	process_dollar_arg(char *stripped_arg, char *res_i, t_exec *exec,
		char *processed_arg)
{
	char	quote_type;
	int		total_quotes;

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
		return (process_quoted_dollar(stripped_arg, processed_arg, exec));
	handle_processed_arg(processed_arg, stripped_arg);
	return (0);
}

int	check_space_conditions(char **res, int res_index,
		int *prev_was_quote)
{
	int	next_is_quote;
	int	curr_is_quote;

	next_is_quote = (res[res_index + 1] && (res[res_index + 1][0] == '"'
				|| res[res_index + 1][0] == '\''));
	curr_is_quote = (res[res_index][0] == '"' || res[res_index][0] == '\'');
	if (res[res_index + 1] && res[res_index + 1][0] != '>' && res[res_index
		+ 1][0] != '<' && !*prev_was_quote && !next_is_quote && !curr_is_quote)
		return (1);
	*prev_was_quote = curr_is_quote || next_is_quote;
	return (0);
}
