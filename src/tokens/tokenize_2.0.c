/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:13:53 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/29 11:33:14 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_token_info(t_token *token)
{
	const char	*type_str[] = {"WORD", "COMMAND", "REDIRECT_IN", "REDIRECT_OUT",
		"APPEND", "PIPE", "HEREDOC", "OR", "SEMICOLON", "PAREN_L",
		"PAREN_R"};

	printf("\033[1;34mToken:\033[0m '%s'\n", token->value);
	printf("\033[1;32mType:\033[0m %s (%d)\n", type_str[token->type],
		token->type);
	printf("------------------------\n");
}
t_token *create_token(char *value, t_token_type type)
{
    t_token *token;
    char *dup_value;

    if (!value)
        return (NULL);

    // Step 1: Create strdup first
    dup_value = ft_strdup(value);
    if (!dup_value)
        return (NULL);

    // Step 2: Create token
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
    {
        free(dup_value); // If token malloc fails, free the strdup
        return (NULL);
    }

    // Step 3: Initialize all fields
    token->value = dup_value;
    token->type = type;
    token->next = NULL;
    token->res = NULL;
    token->echo_counter = 0;
    token->is_echo = 0;  // Initialize all fields

    return (token);
}
// t_token *create_token(char *value, t_token_type type)
// {
//     t_token *token;
//     token = malloc(sizeof(t_token));
//     if (!token)
//         return (NULL);
//     token->value = ft_strdup(value);
//     if (!token->value)
//     {
//         free(token);
//         return (NULL);
//     }
//     token->type = type;
//     token->next = NULL;
//     token->res = NULL;
//     token->echo_counter = 0;
//     return (token);
// }
void add_token(t_token **list, t_token *new)
{
    t_token *current;

    if (!list || !new)
        return;

    if (!*list)
    {
        *list = new;
        return;
    }

    current = *list;
    while (current->next)
        current = current->next;
    current->next = new;
}
// void	add_token(t_token **list, t_token *new)
// {
// 	t_token	*current;

// 	if (!list || !new)
// 		return ;
// 	if (!*list)
// 		*list = new;
// 	else
// 	{
// 		current = *list;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new;
// 	}
// }
