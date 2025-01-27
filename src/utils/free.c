/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/27 13:05:29 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	*free_and_return(char **array, void *return_value)
{
	free_array(array);
	return (return_value);
}

int	free_array_and_return(char **array, int return_value)
{
	free_array(array);
	return (return_value);
}

int	free_word_and_return(char *word, int return_value)
{
	free(word);
	return (return_value);
}

void	free_token_list(t_token *tokens)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		if (tokens->res)
			free_array(tokens->res);
		free(tokens);
		tokens = tmp;
	}
}
