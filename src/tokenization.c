/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:32 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/26 15:40:56 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char tokenize(char *prompt)
{
	int i;
	int single_quote_count;
	int double_quote_count;

	i = 0;
	single_quote_count = count_quotes(prompt, 39);
	double_quote_count = count_quotes(prompt, 34);
	
	if (single_quote_count == 0 && double_quote_count == 0)
		return (ft_split(prompt, ' '));
	if (single_quote_count % 2 != 0)
		return (ft_printf("Uneven amount of single quotes\n"), 1);
	if (double_quote_count % 2 != 0)
		return (ft_printf("Uneven amount of double quotes\n"), 1);
	// the 2 last if conditions are not correct, because if will still return an error if there
	// an uneven amount of single quote inside an even pair of even quotes


		
	// if you find a double quote first, you have to continue parsing until you find another double quote,
	// it doesn´t matter the amount of single quotes that you find in between,
	// you have to treat what's inide the double quotes as one entity.
	// So that means that I want to split at every spaces, until I encounter a quote.
	// Then I have to ignore spaces, and split the next time I encounter the next quote.
}
