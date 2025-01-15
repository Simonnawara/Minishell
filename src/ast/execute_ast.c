/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/15 12:11:01 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

static void uptdate_last_status(t_exec *exec, int status)
{
    exec->last_status = status;
}


// // Function to initialize pipe nodes from a single args array
// int	init_pipe_nodes(t_ast_node *node, char **args)
// {
//     int		left_count;
//     int		pipe_pos;
//     int		right_count;

//     if (!node || !args)
//         return (0);

//     // Find position of pipe and count args
//     pipe_pos = 0;
//     while (args[pipe_pos] && strcmp(args[pipe_pos], "|") != 0)
//         pipe_pos++;
    
//     left_count = pipe_pos;
//     if (args[pipe_pos]) // if pipe found
//         right_count = count_args_until_pipe(args, pipe_pos + 1);
//     else
//         return (0); // No pipe found

//     // Initialize left node
//     node->left = create_node();
//     if (!node->left)
//         return (0);
//     node->left->type = T_COMMAND;
//     node->left->args = copy_args_portion(args, 0, left_count);
//     if (!node->left->args)
//     {
//         free(node->left);
//         node->left = NULL;
//         return (0);
//     }

//     // Initialize right node
//     node->right = create_node();
//     if (!node->right)
//     {
//         free_node(node->left);
//         node->left = NULL;
//         return (0);
//     }
//     node->right->type = T_COMMAND;
//     node->right->args = copy_args_portion(args, pipe_pos + 1, right_count);
//     if (!node->right->args)
//     {
//         free_node(node->left);
//         node->left = NULL;
//         free(node->right);
//         node->right = NULL;
//         return (0);
//     }

//     return (1);
// }

// // Function to free AST node
// void	free_node(t_ast_node *node)
// {
//     if (!node)
//         return;

//     if (node->args)
//     {
//         int i = 0;
//         while (node->args[i])
//         {
//             free(node->args[i]);
//             i++;
//         }
//         free(node->args);
//     }

//     free_node(node->left);
//     free_node(node->right);
//     free(node);
// }

int execute_ast(t_ast_node *ast, t_exec *exec)
{
	
    int status;

    if (!ast)
		return (0);
	if (exec->compteur_pipe > 0)
	{
		
		printf("We are about to enter pipe\n");
        //init_pipe_nodes(ast, ast->args);
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
