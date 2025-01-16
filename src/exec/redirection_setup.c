/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:02:02 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/16 11:01:31 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

	if (!cmd->outfile)
		return (0);
	flags = O_WRONLY | O_CREAT;
	flags |= (cmd->append) ? O_APPEND : O_TRUNC;
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
// static void	print_error(char *prefix, char *file, char *message)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	if (prefix)
// 	{
// 		ft_putstr_fd(prefix, 2);
// 		ft_putstr_fd(": ", 2);
// 	}
// 	if (file)
// 		ft_putstr_fd(file, 2);
// 	if (message)
// 		ft_putstr_fd(message, 2);
// 	ft_putchar_fd('\n', 2);
// }

// static int	save_std_fds(int *saved_stdin, int *saved_stdout)
// {
// 	*saved_stdin = dup(STDIN_FILENO);
// 	if (*saved_stdin == -1)
// 		return (print_error(NULL, NULL, "Failed to save stdin"), -1);
// 	*saved_stdout = dup(STDOUT_FILENO);
// 	if (*saved_stdout == -1)
// 	{
// 		close(*saved_stdin);
// 		return (print_error(NULL, NULL, "Failed to save stdout"), -1);
// 	}
// 	return (0);
// }

// static void	restore_std_fds(int saved_stdin, int saved_stdout)
// {
// 	if (saved_stdin != -1)
// 	{
// 		dup2(saved_stdin, STDIN_FILENO);
// 		close(saved_stdin);
// 	}
// 	if (saved_stdout != -1)
// 	{
// 		dup2(saved_stdout, STDOUT_FILENO);
// 		close(saved_stdout);
// 	}
// }

// static int	setup_input(t_command_table *cmd)
// {
// 	int	fd;

// 	if (!cmd->infile)
// 		return (0);
// 	fd = open(cmd->infile, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		if (access(cmd->infile, F_OK) == -1)
// 			print_error(cmd->infile, NULL, ": No such file or directory");
// 		else if (access(cmd->infile, R_OK) == -1)
// 			print_error(cmd->infile, NULL, ": Permission denied");
// 		else
// 			print_error(cmd->infile, NULL, ": Failed to open file");
// 		return (-1);
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		close(fd);
// 		return (print_error(cmd->infile, NULL, ": Failed to redirect input"), -1);
// 	}
// 	close(fd);
// 	return (0);
// }

// static int	setup_output(t_command_table *cmd)
// {
// 	int	fd;
// 	int	flags;

// 	if (!cmd->outfile)
// 		return (0);
// 	flags = O_WRONLY | O_CREAT;
// 	flags |= (cmd->append) ? O_APPEND : O_TRUNC;
// 	fd = open(cmd->outfile, flags, 0644);
// 	if (fd == -1)
// 	{
// 		if (access(cmd->outfile, F_OK) == 0 && access(cmd->outfile, W_OK) == -1)
// 			print_error(cmd->outfile, NULL, ": Permission denied");
// 		else if (access(cmd->outfile, F_OK) == -1 && 
// 				access(dirname(cmd->outfile), W_OK) == -1)
// 			print_error(cmd->outfile, NULL, ": Directory permission denied");
// 		else
// 			print_error(cmd->outfile, NULL, ": Failed to open file");
// 		return (-1);
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		close(fd);
// 		return (print_error(cmd->outfile, NULL, ": Failed to redirect output"), -1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	setup_redirection(t_command_table *cmd)
// {
// 	int	saved_stdin;
// 	int	saved_stdout;
// 	int	status;

// 	if (!cmd)
// 		return (-1);
// 	if (save_std_fds(&saved_stdin, &saved_stdout) == -1)
// 		return (-1);
// 	status = 0;
// 	if (setup_input(cmd) == -1)
// 		status = -1;
// 	if (status != -1 && setup_output(cmd) == -1)
// 		status = -1;
// 	if (status == -1)
// 		restore_std_fds(saved_stdin, saved_stdout);
// 	else
// 	{
// 		close(saved_stdin);
// 		close(saved_stdout);
// 	}
// 	return (status);
// }
