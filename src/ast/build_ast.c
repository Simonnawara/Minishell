/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:51:36 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*handle_redirection_sequence(t_token **tokens)
{
	t_token		*current;
	t_ast_node	*cmd_node;
	t_ast_node	*root;
	t_ast_node	*prev_redir;
	t_ast_node	*redir;

	current = *tokens;
	cmd_node = NULL;
	root = NULL;
	prev_redir = NULL;
	while (current && current->type != T_COMMAND)
		current = current->next;
	if (current)
	{
		cmd_node = build_command_node(&current);
		if (!cmd_node)
			return (NULL);
	}
	current = *tokens;
	while (current)
	{
		if ((current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT
				|| current->type == T_APPEND || current->type == T_HEREDOC)
			&& current->next)
		{
			redir = create_ast_node(current->type, current->value);
			if (!redir)
			{
				return (NULL);
				free_ast(root);
				free_ast_node(cmd_node);
			}
			if (!current->next || current->next->type != T_WORD)
			{
				free_ast_node(redir);
				free_ast(root);
				free_ast_node(cmd_node);
				return (NULL);
			}
			redir->right = create_ast_node(T_WORD, current->next->value);
			if (!redir->right)
			{
				free_ast_node(redir);
				free_ast(root);
				free_ast_node(cmd_node);
				return (NULL);
			}
			if (!root)
			{
				root = redir;
				if (cmd_node)
					root->left = cmd_node;
			}
			else if (prev_redir)
			{
				prev_redir->left = redir;
				if (cmd_node && !redir->left)
					redir->left = cmd_node;
			}
			prev_redir = redir;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	return (root ? root : cmd_node);
}

t_ast_node	*build_ast(t_token **tokens)
{
	t_token		*current;
	t_ast_node	*pipe;
	t_token		*left;
	t_token		*right;
	t_token		*split;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	while (current)
	{
		if (current->type == T_PIPE)
		{
			pipe = create_ast_node(T_PIPE, "|");
			if (!pipe)
				return (NULL);
			left = *tokens;
			right = current->next;
			split = *tokens;
			while (split && split->next != current)
				split = split->next;
			if (split)
				split->next = NULL;
			pipe->left = build_ast(&left);
			pipe->right = build_ast(&right);
			if (!pipe->left || !pipe->right)
			{
				free_ast_node(pipe->left);
				free_ast_node(pipe->right);
				free_ast_node(pipe);
				return (NULL);
			}
			*tokens = NULL;
			return (pipe);
		}
		current = current->next;
	}
	return (handle_redirection_sequence(tokens));
}
