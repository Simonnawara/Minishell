/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:37 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/23 12:44:46 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_quotes(char *word, char quote)
{
	int	i;
	int	quote_count;

	if (!word)
		return (0);
	i = 0;
	quote_count = 0;
	while (word[i])
	{
		if (word[i] == quote)
			quote_count++;
		i++;
	}
	return (quote_count);
}

int	get_quote_type(char start_quote, char end_quote)
{
	int	quote_type;

	if (start_quote == end_quote)
	{
		if (start_quote == 34 || start_quote == 39)
			return (start_quote);
	}
	if (start_quote == 34 || end_quote == 34)
		quote_type = 34;
	else if (start_quote == 39 || end_quote == 39)
		quote_type = 39;
	else
		quote_type = 0;
	return (quote_type);
}
