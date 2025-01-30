/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/29 11:32:58 by trouilla         ###   ########.fr       */
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

void cleanup_token(t_token *token)
{
    if (!token)
        return;

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

void free_token_list(t_token *tokens)
{
    t_token *next;
    
    while (tokens)
    {
        next = tokens->next;
        
        // Free the value first
        if (tokens->value)
        {
            free(tokens->value);
            tokens->value = NULL;
        }
        
        // Only free res if it's owned by this token
        if (tokens->res && tokens->is_echo)
        {
            free_array(tokens->res);
            tokens->res = NULL;
        }
        
        // Free the token itself
        free(tokens);
        tokens = next;
    }
}
