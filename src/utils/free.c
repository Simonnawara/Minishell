/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:50:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 16:38:46 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
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

char	**free_split(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

void	free_token_list(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	free(node->value);
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
