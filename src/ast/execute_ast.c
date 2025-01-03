/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/03 14:01:37 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

void execute_ast(t_ast_node *root, char **env)
{
    if (!root)
        return;

    if (root->type == T_COMMAND)
        execute_command(root, env);
    else if (root->type == T_PIPE)
        execute_pipe(root->left, root->right, env);
    else if (root->type == T_REDIRECT_IN)
    {
        int fd = open(root->right->value, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            g_exit_status = 1;
            return;
        }
        int saved_stdin = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        close(fd);
        execute_ast(root->left, env);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    else if (root->type == T_REDIRECT_OUT || root->type == T_APPEND)
    {
        int flags = (root->type == T_REDIRECT_OUT) ? 
                   (O_WRONLY | O_CREAT | O_TRUNC) : 
                   (O_WRONLY | O_CREAT | O_APPEND);
        int fd = open(root->right->value, flags, 0644);
        if (fd < 0)
        {
            perror("open");
            g_exit_status = 1;
            return;
        }
        int saved_stdout = dup(STDOUT_FILENO);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execute_ast(root->left, env);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
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
