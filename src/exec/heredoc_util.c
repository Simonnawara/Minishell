/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:42:08 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:01:02 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_heredoc_filename(void)
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

int write_to_heredoc(int fd, char *delimiter)
{
    char *line;
    
    setup_heredoc_signals();
    g_exit_status = 0;  // Reset exit status at start
    
    while (1)
    {
        line = readline("heredoc> ");
        
        // Check for SIGINT or EOF
        if (!line)
        {
            // If it was a SIGINT
            if (g_exit_status == 130)
            {
                setup_signals();
                return (-1);
            }
            // If it was an actual EOF
            ft_putstr_fd("\nwarning: here-document delimited by end-of-file\n", 2);
            setup_signals();
            return (0);
        }
        
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    
    setup_signals();
    return (0);
}

void	cleanup_heredoc(t_heredoc *hd)
{
	if (hd->fd > 0)
		close(hd->fd);
	if (hd->filename)
	{
		unlink(hd->filename);
		free(hd->filename);
	}
}

int setup_heredoc(char *delimiter, t_heredoc *hd)
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

int	count_heredocs(t_ast_node *ast)
{
	int			count;
	t_ast_node	*current;

	count = 0;
	current = ast;
	while (current && current->type == T_HEREDOC)
	{
		count++;
		current = current->left;
	}
	return (count);
}
