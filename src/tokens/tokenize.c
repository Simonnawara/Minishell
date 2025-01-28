/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:52:38 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && isspace(str[i]))
			i++;
		if (!str[i])
			break ;
		count++;
		if (!process_word_for_count(str, &i))
			return (-1);
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

static int	initialize_tokenize(char *prompt, int *pos, int *word_count)
{
	if (!prompt)
		return (0);
	*pos = 0;
	while (prompt[*pos] && isspace(prompt[*pos]))
		(*pos)++;
	*word_count = count_words(prompt);
	if (*word_count < 0)
	{
		ft_printf("Error: Unclosed quote\n");
		return (0);
	}
	return (1);
}

char	**tokenize(char *prompt)
{
	char	**tokens;
	int		word_count;
	int		pos;
	int		token_idx;

	if (!initialize_tokenize(prompt, &pos, &word_count))
		return (NULL);
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
			return (handle_tokenize_error(tokens, token_idx));
		token_idx++;
	}
	tokens[token_idx] = NULL;
	return (tokens);
}
