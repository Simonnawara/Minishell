/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/20 09:31:13 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_heredoc_filename(void)
{
    static int	index;
    char		*name;
    char		*number;

    index++;
    number = ft_itoa(index);
    if (!number)
        return (NULL);
    name = ft_strjoin("/tmp/.heredoc_", number);
    free(number);
    return (name);
}

static int	write_to_heredoc(int fd, char *delimiter)
{
    char	*line;

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

static void	cleanup_heredoc(t_heredoc *hd)
{
    if (hd->fd > 0)
        close(hd->fd);
    if (hd->filename)
    {
        unlink(hd->filename);
        free(hd->filename);
    }
}

static int	setup_heredoc(char *delimiter, t_heredoc *hd)
{
    int	write_fd;

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

static int	collect_heredocs(t_ast_node *ast, t_heredoc **hds, int *count)
{
    if (!ast)
        return (0);
    
    if (ast->type == T_HEREDOC)
    {
        t_heredoc *new_hds = malloc(sizeof(t_heredoc) * (*count + 1));
        if (!new_hds)
            return (1);
        
        if (*hds)
        {
            ft_memcpy(new_hds, *hds, sizeof(t_heredoc) * *count);
            free(*hds);
        }
        *hds = new_hds;
        
        if (setup_heredoc(ast->right->value, &(*hds)[*count]) != 0)
            return (1);
        
        (*count)++;
        
        if (ast->left && ast->left->type == T_HEREDOC)
            return (collect_heredocs(ast->left, hds, count));
    }
    return (0);
}

static void	cleanup_all_heredocs(t_heredoc *hds, int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        cleanup_heredoc(&hds[i]);
        i++;
    }
    free(hds);
}

int	execute_heredoc(t_ast_node *ast, t_exec *exec)
{
    t_heredoc	*heredocs;
    int				heredoc_count;
    int				status;
    int				saved_stdin;

    heredocs = NULL;
    heredoc_count = 0;
    if (!ast || !ast->right || !ast->right->value)
        return (ft_putstr_fd("minishell: syntax error\n", 2), 1);

    if (collect_heredocs(ast, &heredocs, &heredoc_count) != 0)
        return (ft_putstr_fd("minishell: heredoc: error\n", 2), 1);

    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        cleanup_all_heredocs(heredocs, heredoc_count);
        return (1);
    }

    if (dup2(heredocs[heredoc_count - 1].fd, STDIN_FILENO) == -1)
    {
        close(saved_stdin);
        cleanup_all_heredocs(heredocs, heredoc_count);
        return (1);
    }

    if (ast->left && ast->left->type != T_HEREDOC)
        status = execute_ast(ast->left, exec);
    else
        status = 0;

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    cleanup_all_heredocs(heredocs, heredoc_count);
    return (status);
}
