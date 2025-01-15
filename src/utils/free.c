/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/13 11:01:52 by trouilla         ###   ########.fr       */
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

	while (tokens)
	{
		tmp = tokens->next;
		if(tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void free_ast_node(t_ast_node *node)
{
    if (!node)
        return;

    if (node->value)
        free(node->value);

    if (node->args)
    {
        for (int i = 0; node->args[i]; i++)
            free(node->args[i]);
        free(node->args);
    }

    free_ast_node(node->left);
    free_ast_node(node->right);
    free(node);
}

void	free_ast(t_ast_node *root)
{
	if (!root)
		return ;
	free_ast(root->left);
	free_ast(root->right);
	free_ast_node(root);
}

void	free_command_table(t_command_table *cmd)
{
	int	i;

	if (!cmd)
		return ;
	free(cmd->cmd);
	i = 0;
	while (cmd->args && cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd);
}
