/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 16:52:20 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node *create_ast_node(t_token_type type, char *value)
{
	t_ast_node *node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node *build_command_node(t_token **tokens)
{
	t_ast_node *cmd_node;
	t_token *current;

	current = *tokens;
	cmd_node = create_ast_node(T_COMMAND, current->value);
	if (!cmd_node)
		return (NULL);
	current = current->next;
	while(current && current->type == T_WORD)
	{
		if (!add_argument_to_command(cmd_node, current->value))
			return (free_ast_node(cmd_node), NULL);
		current = current->next;
	}
	*tokens = current;
	return (cmd_node);
}

t_ast_node	*build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_token		*current;

	current = *tokens;
	if (!current)
		return (NULL);
	if (current->type == T_COMMAND)
		return (build_command_node(tokens));
	if (current->type == T_PIPE || current->type == T_AND
		|| current->type == T_OR)
	{
		root = create_ast_node(current->type, current->value);
		if (!root)
			return (NULL);
		*tokens = current->next;
		root->left = build_ast(tokens);
		if (!root->left)
			return (free_ast_node(root), NULL);
		root->right = build_ast(tokens);
		if (!root->right)
			return (free_ast_node(root), NULL);
		return (root);
	}
	return (NULL);
}
