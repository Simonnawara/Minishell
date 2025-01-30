/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:21:06 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 09:56:50 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	free(cmd->heredoc_file);
	free(cmd->delimiter);
	if (cmd->all_outfiles)
	{
		i = 0;
		while (i < cmd->num_outfiles)
			free(cmd->all_outfiles[i++]);
		free(cmd->all_outfiles);
	}
	free(cmd);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}

void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_ast_node(node->left);
	if (node->right)
		free_ast_node(node->right);
	if (node->value)
		free(node->value);
	if (node->args)
		free_array(node->args);
	if (node)
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

void	file_error(char *filename)
{
	write(2, filename, ft_strlen(filename));
	perror(" ");
}
