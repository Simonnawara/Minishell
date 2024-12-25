#include "../includes/minishell.h"

int count_quotes(char *word, char quote)
{
	int i;
	int quote_count;

	i = 0;
	quote_count = 0;
	while (word[i])
	{
		if (word[i] == quote)
			quote_count++;
		i++;
	}
	if (quote_count % 2 == 0)
		return (quote_count);
	return (1);
}

int count_start_quotes(char *word, char quote)
{
	int i;

	i = 0;
	while (word[i] && word[i] == quote)
		i++;
	return (i);
}

int count_end_quotes(char *word, char quote)
{
	int i;
	int quote_count;

	i = ft_strlen(word) - 1;
	quote_count = 0;
	while (i >= 0 && word[i] == quote)
	{
		quote_count++;
		i--;
	}
	return (quote_count);
}