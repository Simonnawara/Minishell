/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/17 14:18:38 by trouilla         ###   ########.fr       */
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

static int	prepare_heredoc(char *delimiter, int *fd_read)
{
	char	*filename;
	int		fd;

	filename = get_heredoc_filename();
	if (!filename)
		return (1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (1);
	}
	if (write_to_heredoc(fd, delimiter) != 0)
	{
		close(fd);
		unlink(filename);
		free(filename);
		return (1);
	}
	close(fd);
	*fd_read = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	if (*fd_read == -1)
		return (1);
	return (0);
}

int	execute_heredoc(t_ast_node *ast, t_exec *exec)
{
	int		status;
	int		fd_read;
	int		saved_stdin;

	if (!ast || !ast->right || !ast->right->value)	
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	if (prepare_heredoc(ast->right->value, &fd_read) != 0)
		return (ft_putstr_fd("minishell: heredoc: temp file error\n", 2), 1);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		close(fd_read);
		return (1);
	}
	if (dup2(fd_read, STDIN_FILENO) == -1)
	{
		close(fd_read);
		close(saved_stdin);
		return (1);
	}
	close(fd_read);
	if (ast->left)
		status = execute_ast(ast->left, exec);
	else
		status = 0;
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (status);
}
