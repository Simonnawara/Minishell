/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 11:06:42 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*process_token_ast(t_token *current, t_ast_node **root,
		t_ast_node *cmd_node, t_ast_node **prev_redir)
{
	t_ast_node	*redir;

	redir = create_redirection_node(current, root, cmd_node);
	if (!redir)
		return (NULL);
	if (!*root)
		handle_root_assignment(root, redir, cmd_node);
	else if (*prev_redir)
		handle_prev_redir(*prev_redir, redir, cmd_node);
	*prev_redir = redir;
	return (current->next->next);
}

static void	process_redirections(t_token *current, t_ast_node **root,
		t_ast_node *cmd_node, t_ast_node **prev_redir)
{
	while (current)
	{
		if ((current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT
				|| current->type == T_APPEND || current->type == T_HEREDOC)
			&& current->next)
			current = process_token_ast(current, root, cmd_node, prev_redir);
		else
			current = current->next;
	}
}

t_ast_node	*handle_redirection_sequence(t_token **tokens)
{
	t_token		*current;
	t_ast_node	*cmd_node;
	t_ast_node	*root;
	t_ast_node	*prev_redir;

	cmd_node = NULL;
	root = NULL;
	prev_redir = NULL;
	current = *tokens;
	while (current && current->type != T_COMMAND)
		current = current->next;
	cmd_node = build_command_node(&current);
	if (current && !(cmd_node))
		return (NULL);
	current = *tokens;
	process_redirections(current, &root, cmd_node, &prev_redir);
	if (root)
		return (root);
	return (cmd_node);
}

t_ast_node	*build_ast(t_token **tokens)
{
	t_token	*current;
	t_token	*split;

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
