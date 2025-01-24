/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/24 13:28:26 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == ';'
		|| c == '&' || c == '(' || c == ')');
}

static int	get_operator_len(const char *str)
{
	if ((str[0] == '>' && str[1] == '>')
		|| (str[0] == '<' && str[1] == '<')
		|| (str[0] == '&' && str[1] == '&')
		|| (str[0] == '|' && str[1] == '|'))
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
	char	quote_type;
	char	*word;

	start = *pos;
	len = 0;
	if (str[start] == '"' || str[start] == '\'')
	{
		quote_type = str[start];
		len++;
		while (str[start + len] && str[start + len] != quote_type)
			len++;
		if (str[start + len] == quote_type)
			len++;
		else
		{
			ft_putendl_fd("Error: Unclosed quote", 2);
			return (NULL);
		}
	}
	else if (is_operator(str[start]))
		len = get_operator_len(str + start);
	else
	{
		while (str[start + len] && !isspace(str[start + len])
			&& !is_operator(str[start + len])
			&& str[start + len] != '"' && str[start + len] != '\'')
			len++;
	}
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
