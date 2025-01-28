/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 15:18:34 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*create_redirection_node(t_token *current,
					t_ast_node **root, t_ast_node *cmd_node)
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

static void	handle_root_assignment(t_ast_node **root, t_ast_node *redir,
				t_ast_node *cmd_node)
{
	*root = redir;
	if (cmd_node)
		(*root)->left = cmd_node;
}

static void	handle_prev_redir(t_ast_node *prev_redir, t_ast_node *redir,
				t_ast_node *cmd_node)
{
	prev_redir->left = redir;
	if (cmd_node && !redir->left)
		redir->left = cmd_node;
}

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
			redir = create_redirection_node(current, &root, cmd_node);
			if (!redir)
				return (NULL);
			if (!root)
				handle_root_assignment(&root, redir, cmd_node);
			else if (prev_redir)
				handle_prev_redir(prev_redir, redir, cmd_node);
			prev_redir = redir;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	if (root)
		return (root);
	return (cmd_node);
}


t_ast_node	*build_ast(t_token **tokens)
{
	t_token		*current;
	t_token		*split;

	if (!tokens || !*tokens)
		return (NULL);
	current = *tokens;
	while (current)
	{
		if (current->type == T_PIPE)
		{
			split = *tokens;
			while (split && split->next != current)
				split = split->next;
			return (handle_pipe_creation(tokens, current, split));
		}
		current = current->next;
	}
	return (handle_redirection_sequence(tokens));
}

