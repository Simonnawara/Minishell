/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/14 16:48:58 by sinawara         ###   ########.fr       */
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
	if (exec->compteur_pipe > 0)
	{
		
		printf("We are about to enter pipe\n");
       	status = execute_pipe_node(ast, exec);
       	uptdate_last_status(exec, status);
		exec->compteur_pipe--;
		return (status);
	}
//	printf("ast->type : %d\n", ast->type);
//	printf("ast->value : %s\n", ast->value);

	//if (ast->type == T_PIPE)
    //{
        //printf("la\n");
        //return (status);
    //}

    if (ast->type == T_COMMAND)
    {
		//printf("AST VALUE : %s\n", ast->args[1]);
        status = execute_simple_command(ast, exec);
        uptdate_last_status(exec, status);
        return (status);
    }
    return (1);
}
