/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:13:53 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/23 13:21:55 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_token_info(t_token *token)
{
    const char *type_str[] = {
        "WORD", "COMMAND", "REDIRECT_IN", "REDIRECT_OUT",
        "APPEND", "PIPE", "HEREDOC", "OR", "SEMICOLON",
        "PAREN_L", "PAREN_R"
    };

    printf("\033[1;34mToken:\033[0m '%s'\n", token->value);
    printf("\033[1;32mType:\033[0m %s (%d)\n", type_str[token->type], token->type);
    printf("------------------------\n");
}


t_token	*create_token(char *value, t_token_type type)
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
	token->res = NULL;  // Initialize additional members
    token->echo_counter = 0;
	return (token);
}

void add_token(t_token **list, t_token *new)
{
    t_token *current;

    if (!list || !new)
        return;
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
