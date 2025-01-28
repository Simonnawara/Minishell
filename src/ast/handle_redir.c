/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:08:08 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 15:39:45 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*create_redirection_node(t_token *current, t_ast_node **root,
		t_ast_node *cmd_node)
{
	t_ast_node	*redir;

	redir = create_ast_node(current->type, current->value);
	if (!redir)
	{
		free_ast(*root);
		free_ast_node(cmd_node);
		return (NULL);
	}
	if (!current->next || current->next->type != T_WORD)
	{
		free_ast_node(redir);
		free_ast(*root);
		free_ast_node(cmd_node);
		return (NULL);
	}
	redir->right = create_ast_node(T_WORD, current->next->value);
	if (!redir->right)
	{
		free_ast_node(redir);
		free_ast(*root);
		free_ast_node(cmd_node);
		return (NULL);
	}
	return (redir);
}

void	handle_root_assignment(t_ast_node **root, t_ast_node *redir,
		t_ast_node *cmd_node)
{
	*root = redir;
	if (cmd_node)
		(*root)->left = cmd_node;
}

void	handle_prev_redir(t_ast_node *prev_redir, t_ast_node *redir,
		t_ast_node *cmd_node)
{
	prev_redir->left = redir;
	if (cmd_node && !redir->left)
		redir->left = cmd_node;
}
