/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/23 14:23:58 by trouilla         ###   ########.fr       */
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
    t_ast_node *cmd_node;
    t_command_table cmd = {0};
    int saved_stdin;
    int last_heredoc_fd = -1;

    if (!ast || !ast->right || !ast->right->value)
        return (ft_putstr_fd("minishell: syntax error\n", 2), 1);

    cmd_node = get_command_node(ast);
    heredoc_count = collect_heredocs(ast, &heredocs);
    if (heredoc_count < 0)
        return (ft_putstr_fd("minishell: heredoc: error\n", 2), 1);

    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1 || heredoc_count == 0)
        return (cleanup_all_heredocs(heredocs, heredoc_count), 1);

    // Get last heredoc's fd
    if (heredoc_count > 0)
        last_heredoc_fd = heredocs[heredoc_count - 1].fd;

    if (cmd_node)
    {
        t_ast_node *current = cmd_node;
        cmd.cmd = current->value;
        cmd.args = current->args;

        // Set up input from last heredoc if no file redirection
        if (last_heredoc_fd != -1)
        {
            dup2(last_heredoc_fd, STDIN_FILENO);
            close(last_heredoc_fd);
        }

        // Override with file input if present
        while (current && current->type == T_REDIRECT_IN)
        {
            cmd.infile = current->right->value;
            current = current->left;
        }

        if (cmd.infile)
        {
            int fd = open(cmd.infile, O_RDONLY);
            if (fd != -1)
            {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
        }

        status = execute_simple_command(current, exec, cmd);
    }
    else
        status = 0;

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    cleanup_all_heredocs(heredocs, heredoc_count);

    return (status);
}
// int execute_heredoc(t_ast_node *ast, t_exec *exec)
// {
//     t_heredoc *heredocs;
//     int heredoc_count;
//     int status;
//     t_ast_node *cmd_node;
//     t_command_table cmd = {0};
//     int saved_stdin;

//     if (!ast || !ast->right || !ast->right->value)
//         return (ft_putstr_fd("minishell: syntax error\n", 2), 1);

//     cmd_node = get_command_node(ast);
//     heredoc_count = collect_heredocs(ast, &heredocs);
//     if (heredoc_count < 0)
//         return (ft_putstr_fd("minishell: heredoc: error\n", 2), 1);

//     saved_stdin = dup(STDIN_FILENO);
//     if (saved_stdin == -1 || heredoc_count == 0)
//         return (cleanup_all_heredocs(heredocs, heredoc_count), 1);

//     if (cmd_node)
//     {
//         t_ast_node *current = cmd_node;

//         // Set up command structure
//         cmd.cmd = current->value;
//         cmd.args = current->args;

//         // Handle file redirection if present
//         while (current && current->type == T_REDIRECT_IN)
//         {
//             cmd.infile = current->right->value;
//             current = current->left;
//         }

//         // Setup redirections using existing function
//         if (setup_redirection(&cmd) == -1)
//         {
//             dup2(saved_stdin, STDIN_FILENO);
//             close(saved_stdin);
//             cleanup_all_heredocs(heredocs, heredoc_count);
//             return (1);
//         }

//         status = execute_simple_command(current, exec, cmd);
//     }
//     else
//         status = 0;

//     dup2(saved_stdin, STDIN_FILENO);
//     close(saved_stdin);
//     cleanup_all_heredocs(heredocs, heredoc_count);

//     return (status);
// }