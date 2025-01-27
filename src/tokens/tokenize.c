/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/27 09:48:00 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == ';' || c == '&' || c == '('
		|| c == ')');
}

static int	get_operator_len(const char *str)
{
	if ((str[0] == '>' && str[1] == '>') || (str[0] == '<' && str[1] == '<')
		|| (str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|'))
		return (2);
	return (1);
}

int	move_past_quotes(const char *str, char quote_type, int *i)
{
	(*i)++;
	while (str[*i] && str[*i] != quote_type)
		(*i)++;
	if (str[*i] == quote_type)
	{
		(*i)++;
		return (1);
	}
	return (0);
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

int	count_words(const char *str)
{
	int		count;
	int		i;
	char	quote_type;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		count++;
		if (str[i] == '"' || str[i] == '\'')
		{
			quote_type = str[i];
			if (!move_past_quotes(str, quote_type, &i))
				return (-1);
			continue ;
		}
		if (is_operator(str[i]))
		{
			i += get_operator_len(str + i);
			continue ;
		}
		while (str[i] && !isspace(str[i]) && str[i] != '"' && str[i] != '\''
			&& !is_operator(str[i]))
			i++;
	}
	return (count);
}

static char	*get_word(const char *str, int *pos)
{
	int		start;
	int		len;
	char	*word;

	start = *pos;
	len = 0;
	if (str[start] == '"' || str[start] == '\'')
	{
		if (!handle_quoted_word(str, &len, start))
			return (NULL);
	}
	else if (is_operator(str[start]))
		len = get_operator_len(str + start);
	else if (!handle_regular_word(str, &len, start))
		return (NULL);
	if (len == 0)
		return (NULL);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, str + start, len + 1);
	*pos += len;
	return (word);
}

char	**tokenize(char *prompt)
{
	char	**tokens;
	int		word_count;
	int		pos;
	int		token_idx;

	if (!prompt)
		return (NULL);
	pos = 0;
	while (prompt[pos] && isspace(prompt[pos]))
		pos++;
	word_count = count_words(prompt);
	if (word_count < 0)
		return (ft_printf("Error: Unclosed quote\n"), NULL);
	tokens = malloc(sizeof(char *) * (word_count + 1));
	if (!tokens)
		return (NULL);
	token_idx = 0;
	while (prompt[pos])
	{
		while (prompt[pos] && isspace(prompt[pos]))
			pos++;
		if (!prompt[pos])
			break ;
		tokens[token_idx] = get_word(prompt, &pos);
		if (!tokens[token_idx])
			free_and_return(tokens, NULL);
		token_idx++;
	}
	tokens[token_idx] = NULL;
	return (tokens);
}
