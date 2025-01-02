/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:13:53 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/03 00:22:11 by sinawara         ###   ########.fr       */
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

static char *extract_quoted_token(char *str, int *i, char quote)
{
    int     start;
    int     len;
    char    *token;

    start = *i + 1;  // Skip opening quote
    (*i)++;
    while (str[*i] && str[*i] != quote)
        (*i)++;
    if (str[*i] == quote)
    {
        len = *i - start;
        (*i)++;  // Skip closing quote
    }
    else
        return NULL;  // Unclosed quote

    token = malloc(sizeof(char) * (len + 1));
    if (!token)
        return NULL;
    ft_strlcpy(token, str + start, len + 1);
    return token;
}

void print_token_info(t_token *token)
{
    const char *type_str[] = {
        "WORD", "COMMAND", "REDIRECT_IN", "REDIRECT_OUT",
        "APPEND", "PIPE", "AND", "OR", "SEMICOLON",
        "PAREN_L", "PAREN_R"
    };

    printf("\033[1;34mToken:\033[0m '%s'\n", token->value);
    printf("\033[1;32mType:\033[0m %s (%d)\n", type_str[token->type], token->type);
    printf("------------------------\n");
}

t_token *tokenize_input(char *input, char **env)
{
    t_token *tokens;
    t_token *new_token;
    char    *temp;
    int     i;

    if (!input)
        return (NULL);
    tokens = NULL;
    i = 0;
    while (input[i])
    {
        while (input[i] && isspace(input[i]))
            i++;
        if (!input[i])
            break;
        if (input[i] == '"' || input[i] == '\'')
        {
            temp = extract_quoted_token(input, &i, input[i]);
            if (!temp)
                return (free_token_list(tokens), NULL);
            new_token = create_token(temp, T_WORD);
            free(temp);
            if (!new_token)
                return (free_token_list(tokens), NULL);
            add_token(&tokens, new_token);
			print_token_info(new_token);
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
            new_token = create_token(temp, classify_token(temp, env));
			printf("using classify_token\n");
            free(temp);
            if (!new_token)
                return (free_token_list(tokens), NULL);
            add_token(&tokens, new_token);
			print_token_info(new_token);
        }
        else
        {
            temp = ft_substr(input, i, get_word_length(input + i));
            if (!temp)
                return (free_token_list(tokens), NULL);
            new_token = create_token(temp, T_WORD);
			printf("using T_WORD\n");
            free(temp);
            if (!new_token)
                return (free_token_list(tokens), NULL);
            add_token(&tokens, new_token);
			print_token_info(new_token);
            i += get_word_length(input + i);
        }
    }
    return (tokens);
}
