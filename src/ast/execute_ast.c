/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/11 15:42:08 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

static void uptdate_last_status(t_exec *exec, int status)
{
    exec->last_status = status;
}

int execute_ast(t_ast_node *ast, t_exec *exec)
{
    int status;

    if (!ast)
        return (0);
    if (ast->type == T_PIPE)
    {
        status = execute_pipe_node(ast, exec);
        uptdate_last_status(exec, status);
        return (status);
    }
    if (ast->type == T_AND || ast->type == T_OR)
    {
        status = execute_logical_node(ast, exec);
        uptdate_last_status(exec, status);
        return (status);
    }
    if (ast->type == T_COMMAND)
    {
        status = execute_simple_command(ast, exec);
        uptdate_last_status(exec, status);
        return (status);
    }
    return (1);
}
