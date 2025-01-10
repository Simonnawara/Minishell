/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logic_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 09:56:19 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 10:06:56 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_and(t_ast_node *node, t_exec *exec)
{
	int left_status;

	left_status = execute_ast(node->left, exec);
	exec->last_status = left_status;
	if (left_status == 0)
		return (execute_ast(node->right, exec));
	return (left_status);
}

static int handle_or(t_ast_node *node, t_exec *exec)
{
	int left_status;

	left_status = execute_ast(node->left, exec);
	exec->last_status = left_status;
	if (exec->last_status != 0)
		return (execute_ast(node->right, exec));
	return (left_status);
}

int execute_logical_node(t_ast_node *node, t_exec *exec)
{
	if (!node || !node->left || !node->right)
		return (1);
	if (node->type == T_AND)
		return (handle_and(node, exec));
	if (node->type == T_OR)
		return (handle_or(node, exec));
	return (1);
}