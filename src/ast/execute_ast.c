/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 18:30:56 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

void	execute_ast(t_ast_node *root, char **env)
{
	if (!root)
		return ;
	if (root->type == T_COMMAND)
		execute_command(root, env);
	else if (root->type == T_PIPE)
	{
		execute_pipe(root->left, root->right, env);
	}
	else if (root->type == T_AND)
	{
		execute_ast(root->left, env);
		if (g_exit_status == 0)
			execute_ast(root->right, env);
	}
	else if (root->type == T_OR)
	{
		execute_ast(root->left, env);
		if (g_exit_status != 0)
			execute_ast(root->right, env);
	}
}