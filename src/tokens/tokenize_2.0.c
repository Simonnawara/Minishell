/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:13:53 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 18:20:39 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_word_length(char *str)
{
	int	len;

	len = 0;
	while (str[len] && !isspace(str[len]) && !ft_strchr("<>|&;'\"", str[len]))
		len++;
	return (len);
}

static t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

static void	add_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!*list)
		*list = new;
	else
	{
		current = *list;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

static char	*extract_quoted_token(char *str, int *i, char quote)
{
	int		start;
	int		len;
	char	*token;

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] == quote)
		(*i)++;
	len = *i - start;
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	ft_strlcpy(token, str + start, len + 1);
	return (token);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	char	*temp;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '"' || input[i] == '\'')
		{
			temp = extract_quoted_token(input, &i, input[i]);
			if (!temp)
				return (free_token_list(tokens), NULL);
			add_token(&tokens, create_token(temp, T_WORD));
			free(temp);
		}
		else if (ft_strchr("<>|&;", input[i]))
		{
			if ((input[i] == '>' && input[i + 1] == '>') ||
				(input[i] == '|' && input[i + 1] == '|') ||
				(input[i] == '&' && input[i + 1] == '&'))
			{
				temp = ft_substr(input, i, 2);
				i += 2;
			}
			else
			{
				temp = ft_substr(input, i, 1);
				i++;
			}
			if (!temp)
				return (free_token_list(tokens), NULL);
			add_token(&tokens, create_token(temp, get_operator_type(temp)));
			free(temp);
		}
		else
		{
			temp = ft_substr(input, i, get_word_length(input + i));
			if (!temp)
				return (free_token_list(tokens), NULL);
			add_token(&tokens, create_token(temp, T_WORD));
			i += ft_strlen(temp);
			free(temp);
		}
	}
	return (tokens);
}
