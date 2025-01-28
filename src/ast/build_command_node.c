/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Build_command_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:31:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 14:38:15 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*init_command_node(t_token *current)
{
	t_ast_node	*cmd_node;

	if (!current)
		return (NULL);
	cmd_node = create_ast_node(T_COMMAND, current->value);
	if (!cmd_node)
		return (NULL);
	cmd_node->res = current->res;
	cmd_node->echo_counter = current->echo_counter;
	if (!add_argument_to_command(cmd_node, current->value))
	{
		free_ast_node(cmd_node);
		return (NULL);
	}
	return (cmd_node);
}

static t_ast_node	*process_command_args(t_token **current,
		t_ast_node *cmd_node)
{
	*current = (*current)->next;
	while (*current && (*current)->type == T_WORD)
	{
		if (!add_argument_to_command(cmd_node, (*current)->value))
		{
			free_ast_node(cmd_node);
			return (NULL);
		}
		*current = (*current)->next;
	}
	return (cmd_node);
}

t_ast_node	*build_command_node(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_token		*current;

	current = *tokens;
	cmd_node = init_command_node(current);
	if (!cmd_node)
		return (NULL);
	cmd_node = process_command_args(&current, cmd_node);
	if (!cmd_node)
		return (NULL);
	*tokens = current;
	return (cmd_node);
}
