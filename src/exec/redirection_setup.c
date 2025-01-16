/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:02:02 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/16 16:04:30 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
static int setup_input(t_command_table *cmd)
{
	int fd;

	if (!cmd->infile)
		return (0);
	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->infile, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int setup_output(t_command_table *cmd)
{
	int	fd;
	int	flags;

	printf("Redirection\n");
	if (!cmd->outfile)
		return (0);
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	//flags |= (cmd->append) ? O_APPEND;
	fd = open(cmd->outfile, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->outfile, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
int setup_redirection(t_command_table *cmd)
{
	if (setup_input(cmd) == -1)
		return (-1);
	if (setup_output(cmd) == -1)
		return (-1);
	return (0);
}
*/


static int	handle_redirect_error(char *filename, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error_msg, 2);
	return (-1);
}

static int	setup_input(t_command_table *cmd, int *original_stdin)
{
	int	fd;

	if (!cmd->infile)
		return (0);
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin == -1)
		return (handle_redirect_error(cmd->infile, "Failed to backup stdin"));
	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		close(*original_stdin);
		return (handle_redirect_error(cmd->infile, "No such file or directory"));
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		close(*original_stdin);
		return (handle_redirect_error(cmd->infile, "Failed to redirect input"));
	}
	close(fd);
	return (0);
}

static int	setup_output(t_command_table *cmd, int *original_stdout)
{
	int	fd;
	int	flags;

	if (!cmd->outfile)
		return (0);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdout == -1)
		return (handle_redirect_error(cmd->outfile, "Failed to backup stdout"));
	flags = O_WRONLY | O_CREAT;
	flags |= (cmd->append) ? O_APPEND : O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd == -1)
	{
		close(*original_stdout);
		return (handle_redirect_error(cmd->outfile, "Permission denied"));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(*original_stdout);
		return (handle_redirect_error(cmd->outfile, "Failed to redirect output"));
	}
	close(fd);
	return (0);
}

static void	restore_fds(int original_stdin, int original_stdout)
{
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	if (original_stdout != -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}

int	setup_redirection(t_command_table *cmd)
{
	int	original_stdin;
	int	original_stdout;
	int	ret;

	original_stdin = -1;
	original_stdout = -1;
	if (!cmd)
		return (-1);
	ret = setup_input(cmd, &original_stdin);
	if (ret == -1)
	{
		restore_fds(original_stdin, original_stdout);
		return (-1);
	}
	ret = setup_output(cmd, &original_stdout);
	if (ret == -1)
	{
		restore_fds(original_stdin, original_stdout);
		return (-1);
	}
	return (0);
}