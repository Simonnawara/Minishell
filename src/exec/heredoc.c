/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:06:24 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*get_command_node(t_ast_node *ast)
{
	t_ast_node	*current;

	current = ast;
	while (current && current->type == T_HEREDOC)
		current = current->left;
	if (current && current->type != T_HEREDOC)
		return (current);
	return (NULL);
}

int collect_heredocs(t_ast_node *ast, t_heredoc **hds)
{
    int total;
    t_heredoc *new_hds;
    t_ast_node *current;
    int i;

    total = count_heredocs(ast);
    if (total == 0)
        return (0);

    new_hds = malloc(sizeof(t_heredoc) * total);
    if (!new_hds)
        return (1);

    current = ast;
    i = 0;
    while (current && current->type == T_HEREDOC && i < total)
    {
        if (setup_heredoc(current->right->value, &new_hds[i]) != 0)
        {
            // Check if it was interrupted by CTRL+C
            if (g_exit_status == 130)
            {
                cleanup_all_heredocs(new_hds, i);
                free(new_hds);
                return (-1);
            }
            return (handle_heredoc_error(new_hds, i));
        }
        i++;
        current = current->left;
    }
    *hds = new_hds;
    return (total);
}

// void	cleanup_all_heredocs(t_heredoc *hds, int count)
// {
// 	int	i;

// 	i = 0;
// 	while (i < count)
// 	{
// 		cleanup_heredoc(&hds[i]);
// 		i++;
// 	}
// 	free(hds);
// }

void cleanup_all_heredocs(t_heredoc *hds, int count)
{
    int i;

    if (!hds)
        return;

    for (i = 0; i < count; i++)
    {
        if (hds[i].fd > 0)
        {
            close(hds[i].fd);
            hds[i].fd = -1;
        }
        if (hds[i].filename)
        {
            unlink(hds[i].filename);
            free(hds[i].filename);
            hds[i].filename = NULL;
        }
    }
}

static int	init_heredoc(t_ast_node *ast, int *heredoc_count,
		t_heredoc **heredocs, int *saved_stdin)
{
	if (!ast || !ast->right || !ast->right->value)
	{
		g_exit_status = 2;
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	}
	*heredoc_count = collect_heredocs(ast, heredocs);
	if (*heredoc_count < 0)
		return (handle_heredoc_error(*heredocs, *heredoc_count));
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1 || *heredoc_count == 0)
		return (cleanup_all_heredocs(*heredocs, *heredoc_count), 1);
	return (0);
}

int execute_heredoc(t_ast_node *ast, t_exec *exec)
{
    t_heredoc *heredocs;
    int heredoc_count;
    int saved_stdin;
    int last_heredoc_fd;
    int status;

    heredocs = NULL;
    last_heredoc_fd = -1;

    // Setup heredocs
    if (init_heredoc(ast, &heredoc_count, &heredocs, &saved_stdin))
    {
        if (g_exit_status == 130)  // SIGINT case
        {
            if (saved_stdin != -1)
            {
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdin);
            }
            if (heredocs)
            {
                cleanup_all_heredocs(heredocs, heredoc_count);
                free(heredocs);
            }
            return (1);  // Return 1 instead of -1 to continue shell
        }
        return (1);
    }

    // If we have heredocs, use the last one's fd
    if (heredoc_count > 0)
        last_heredoc_fd = heredocs[heredoc_count - 1].fd;

    // Only execute command if we weren't interrupted
    if (g_exit_status != 130 && get_command_node(ast))
        status = execute_command_node(get_command_node(ast), exec, last_heredoc_fd);
    else
        status = 1;  // Error status but don't exit

    // Cleanup
    if (saved_stdin != -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    
    if (heredocs)
    {
        cleanup_all_heredocs(heredocs, heredoc_count);
        free(heredocs);
    }

    return (status);
}
