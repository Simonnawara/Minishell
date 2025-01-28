/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:56:01 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == ';' || c == '&'
		|| c == '(' || c == ')');
}

int	get_operator_len(const char *str)
{
	if ((str[0] == '>' && str[1] == '>') || (str[0] == '<' && str[1] == '<')
		|| (str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|'))
		return (2);
	return (1);
}

int	handle_quoted_word(const char *str, int *len, int start)
{
	char	quote_type;

	quote_type = str[start];
	(*len)++;
	while (str[start + *len] && str[start + *len] != quote_type)
		(*len)++;
	if (str[start + *len] == quote_type)
	{
		(*len)++;
		return (1);
	}
	ft_putendl_fd("Error: Unclosed quote", 2);
	g_exit_status = 2;
	return (0);
}

int	handle_regular_word(const char *str, int *len, int start)
{
	while (str[start + *len] && !isspace(str[start + *len])
		&& !is_operator(str[start + *len]) && str[start + *len] != '"'
		&& str[start + *len] != '\'')
		(*len)++;
	return (1);
}

int	process_word_for_count(const char *str, int *i)
{
	char	quote_type;

	if (str[*i] == '"' || str[*i] == '\'')
	{
		quote_type = str[*i];
		if (!move_past_quotes(str, quote_type, i))
			return (0);
	}
	else if (is_operator(str[*i]))
		*i += get_operator_len(str + *i);
	else
	{
		while (str[*i] && !isspace(str[*i]) && str[*i] != '"'
			&& str[*i] != '\'' && !is_operator(str[*i]))
			(*i)++;
	}
	return (1);
}
