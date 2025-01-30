/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 09:54:49 by trouilla         ###   ########.fr       */
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

void	cleanup_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
	{
		free(token->value);
		token->value = NULL;
	}
	if (token->res && token->is_echo)
	{
		free_array(token->res);
		token->res = NULL;
	}
	free(token);
}

void	free_token_list(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		if (tokens->value)
		{
			free(tokens->value);
			tokens->value = NULL;
		}
		if (tokens->res && tokens->is_echo)
		{
			free_array(tokens->res);
			tokens->res = NULL;
		}
		free(tokens);
		tokens = next;
	}
}
