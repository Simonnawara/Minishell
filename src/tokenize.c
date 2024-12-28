/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/28 15:59:14 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int move_past_quotes(const char *str, char quote_type, int *i)
{
    (*i)++;  // Move past the opening quote
    while (str[*i] && str[*i] != quote_type)
        (*i)++;
    if (str[*i] == quote_type)
    {
        (*i)++;  // Move past the closing quote
        return (1);
    }
    return (0);  // Unclosed quote
}


int count_words(const char *str)
{
    int count;
    int i;
    char quote_type;

    count = 0;
    i = 0;

    while (str[i])
    {
        // Skip whitespace
        while (str[i] && isspace(str[i]))
            i++;

        if (!str[i])
            break;

        count++;  // Found start of a new word/quoted string

        // Handle quoted strings
        if (str[i] == '"' || str[i] == '\'')
        {
            quote_type = str[i];
            if (!move_past_quotes(str, quote_type, &i))
                return (-1);  // Error: unclosed quote
            continue;
        }

        // Handle regular words
        while (str[i] && !isspace(str[i]) && str[i] != '"' && str[i] != '\'')
            i++;
    }
    return count;
}

static char *get_word(const char *str, int *pos)
{
    int start;
    int len;
    char quote_type;
    char *word;

    start = *pos;
    len = 0;

    // If it starts with a quote
    if (str[start] == '"' || str[start] == '\'')
    {
        quote_type = str[start];
        len++;  // Include opening quote
        while (str[start + len] && str[start + len] != quote_type)
            len++;
        if (str[start + len] == quote_type)
            len++;  // Include closing quote
    }
    // Regular word
    else
    {
        while (str[start + len] && !isspace(str[start + len]) &&
               str[start + len] != '"' && str[start + len] != '\'')
            len++;
    }

    word = malloc(sizeof(char) * (len + 1));
    if (!word)
        return (NULL);

    ft_strlcpy(word, str + start, len + 1);
    *pos += len;
    return (word);
}



char **tokenize(char *prompt)
{
    char **tokens;
    int word_count;
    int i;
    int pos;
    int token_idx;

    // Skip leading whitespace
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
        // Skip whitespace
        while (prompt[pos] && isspace(prompt[pos]))
            pos++;

        if (!prompt[pos])
            break;

        tokens[token_idx] = get_word(prompt, &pos);
        if (!tokens[token_idx])
        {
            // Cleanup on error
            for (i = 0; i < token_idx; i++)
                free(tokens[i]);
            free(tokens);
            return (NULL);
        }
        token_idx++;
    }
    tokens[token_idx] = NULL;
    return (tokens);
}






