/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/27 15:55:23 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int count_words(const char *str)
{
	int count;
	int in_word;
	char quote;

	count = 0;
	in_word = 0;
	quote = 0;
	
	while(*str)
	{
		if (*str == 34 || *str == 39)
		{
			if (quote == 0)
			{
				quote = *str;
				count++;
				str++;
			}
			else if (*str == quote)
			{
				quote = 0;
				str++;
			}
			else
				str++;
		}
		else if (isspace(*str))
		{
			if (quote == 0)
				in_word = 0;
			str++;
		}
		else
		{
			if (quote == 0 && !in_word)
			{
				count++;
				in_word = 1;
			}
			str++;
		}
	}
	return (count);
}



static char	*get_word(char const *str)
{
	size_t	i;
	char	*res;
	char quote;

	i = 0;
	quote = 0;

	
	while(str[i])
	{
		if ((str[i] == 39 || str[i] == 34) && quote == 0)
		{
			quote = str[i++];
			while(str[i] && str[i] != quote)
				i++;
			if(str[i] == quote)
				i++;
			break ;
		}
		else if (isspace(str[i]) && quote == 0)
			break ;
		else
			i++;
	}
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	ft_strlcat(res, str, i + 1);
	return (res);
	
}



char tokenize(char *prompt)
{
	int i;
	int j;
	int single_quote_count;
	int double_quote_count;
	char **tab;


	single_quote_count = count_quotes(prompt, 39);
	double_quote_count = count_quotes(prompt, 34);
	
	if (single_quote_count == 0 && double_quote_count == 0)
		return (ft_split(prompt, ' '));
	if (single_quote_count % 2 != 0 && double_quote_count % 2 != 0)
		return (ft_printf("Uneven amount of quotes\n"), 1);



		
	// if you find a double quote first, you have to continue parsing until you find another double quote,
	// it doesn´t matter the amount of single quotes that you find in between,
	// you have to treat what's inide the double quotes as one entity.
	// So that means that I want to split at every spaces, until I encounter a quote.
	// Then I have to ignore spaces, and split the next time I encounter the next quote.

	tab = malloc(sizeof(char *) * count_words(prompt) + 1); // +1 for the null terminator at the end
	if (!tab)
		return (NULL);
	
	i = 0;
	j = 0;
	while(prompt[i] && j < count_words(prompt))
	{
		while(prompt[i] == ' ' && prompt[i]) //replace '' by isspace
			i++;
		tab[j] = get_word(prompt + i);
		if (!tab[j])
			return (free_split(tab));
		i += ft_strlen(tab[j]);
		j++;
	}
	tab[j] = NULL;
	return (tab);
	
}



/* int main()
{
    const char *examples[] = {
        "Hello World \"How Are You\"",
        "Hello World 'How Are You'",
        "Hello World \"'How Are You'\"",
        "Hello World 'How Are' \"you\""
    };

    for (int i = 0; i < 4; i++) {
        printf("Input: %s\n", examples[i]);
        printf("Word Count: %d\n\n", count_words(examples[i]));
    }

    return 0;
} */