/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/20 11:11:26 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static char *get_heredoc_filename(void)
{
    static int index;
    char *name;
    char *number;

    index++;
    number = ft_itoa(index);
    if (!number)
        return (NULL);
    name = ft_strjoin("/tmp/.heredoc_", number);
    free(number);
    return (name);
}

static int write_to_heredoc(int fd, char *delimiter)
{
    char *line;

    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            ft_putstr_fd("\nwarning: here-document delimited", 2);
            ft_putstr_fd(" by end-of-file\n", 2);
            return (0);
        }
        if (!ft_strcmp(line, delimiter))
        {
            free(line);
            return (0);
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
}

static void cleanup_heredoc(t_heredoc *hd)
{
    if (hd->fd > 0)
        close(hd->fd);
    if (hd->filename)
    {
        unlink(hd->filename);
        free(hd->filename);
    }
}

static int setup_heredoc(char *delimiter, t_heredoc *hd)
{
    int write_fd;

    hd->filename = get_heredoc_filename();
    if (!hd->filename)
        return (1);
    write_fd = open(hd->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (write_fd == -1)
    {
        free(hd->filename);
        return (1);
    }
    if (write_to_heredoc(write_fd, delimiter) != 0)
    {
        close(write_fd);
        cleanup_heredoc(hd);
        return (1);
    }
    close(write_fd);
    hd->fd = open(hd->filename, O_RDONLY);
    if (hd->fd == -1)
    {
        cleanup_heredoc(hd);
        return (1);
    }
    return (0);
}

static int count_heredocs(t_ast_node *ast)
{
    int count;
    t_ast_node *current;

    count = 0;
    current = ast;
    while (current && current->type == T_HEREDOC)
    {
        count++;
        current = current->left;
    }
    return (count);
}

static t_ast_node *get_command_node(t_ast_node *ast)
{
    t_ast_node *current;

    current = ast;
    while (current && current->type == T_HEREDOC)
        current = current->left;
    
    if (current && current->type != T_HEREDOC)
        return (current);
    return (NULL);
}

static int collect_heredocs(t_ast_node *ast, t_heredoc **hds)
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

    // Always collect all heredocs first
    current = ast;
    i = 0;
    while (current && current->type == T_HEREDOC && i < total)
    {
		 printf("Debug: Processing heredoc %d with delimiter: %s\n", 
               i, current->right->value);
        if (setup_heredoc(current->right->value, &new_hds[i]) != 0)
        {
            while (i > 0)
            {
                i--;
                cleanup_heredoc(&new_hds[i]);
            }
            free(new_hds);
            return (1);
        }
        i++;
        current = current->left;
    }

    *hds = new_hds;
    return (total);
}

static void cleanup_all_heredocs(t_heredoc *hds, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        cleanup_heredoc(&hds[i]);
        i++;
    }
    free(hds);
}

int execute_heredoc(t_ast_node *ast, t_exec *exec)
{
    t_heredoc *heredocs;
    int heredoc_count;
    int status;
    int saved_stdin;
    t_ast_node *cmd_node;

    heredocs = NULL;
    if (!ast || !ast->right || !ast->right->value)
	{
        return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	}
	printf("Debug: Node type: %d\n", ast->type);
    if (ast->left)
        printf("Debug: Left node type: %d\n", ast->left->type);
    if (ast->right)
        printf("Debug: Right node value: %s\n", ast->right->value);
    // Get the command node first (if any)
    cmd_node = get_command_node(ast);

    // Collect ALL heredocs first before any execution
    heredoc_count = collect_heredocs(ast, &heredocs);
    if (heredoc_count < 0)
        return (ft_putstr_fd("minishell: heredoc: error\n", 2), 1);

    if (heredoc_count == 0)
        return (0);

    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        cleanup_all_heredocs(heredocs, heredoc_count);
        return (1);
    }

    // Set input to last heredoc if we have a command, first otherwise
    if (cmd_node)
    {
        if (dup2(heredocs[heredoc_count - 1].fd, STDIN_FILENO) == -1)
        {
            close(saved_stdin);
            cleanup_all_heredocs(heredocs, heredoc_count);
            return (1);
        }
        status = execute_ast(cmd_node, exec);
    }
    else if (heredoc_count > 0)
    {
        if (dup2(heredocs[0].fd, STDIN_FILENO) == -1)
        {
            close(saved_stdin);
            cleanup_all_heredocs(heredocs, heredoc_count);
            return (1);
        }
        status = 0;
    }
    else
        status = 0;

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    cleanup_all_heredocs(heredocs, heredoc_count);

    return (status);
}