/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:02:02 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/23 11:08:37 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_redirect_error(char *filename, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error_msg, 2);
	return (-1);
}

static int setup_input(t_command_table *cmd)
{
    int fd;

    if (!cmd->infile)
        return (0);

    cmd->saved_stdin = dup(STDIN_FILENO);
    if (cmd->saved_stdin == -1)
        return (handle_redirect_error(cmd->infile, "Failed to backup stdin"));

    fd = open(cmd->infile, O_RDONLY);
    if (fd == -1)
        return (handle_redirect_error(cmd->infile, "No such file or directory"));

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        return (handle_redirect_error(cmd->infile, "Failed to redirect input"));
    }

    close(fd);
    return (0);
}

static int setup_output(t_command_table *cmd)
{
    int fd;
    int flags;

    if (!cmd->outfile)
        return (0);

    cmd->saved_stdout = dup(STDOUT_FILENO);
    if (cmd->saved_stdout == -1)
        return (handle_redirect_error(cmd->outfile, "Failed to backup stdout"));

    flags = O_WRONLY | O_CREAT;
    flags |= cmd->append ? O_APPEND : O_TRUNC;

    fd = open(cmd->outfile, flags, 0644);
    if (fd == -1)
        return (handle_redirect_error(cmd->outfile, "Permission denied"));

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        return (handle_redirect_error(cmd->outfile, "Failed to redirect output"));
    }

    close(fd);
    return (0);
}

int setup_redirection(t_command_table *cmd)
{
    if (!cmd)
        return (-1);

    cmd->saved_stdin = -1;
    cmd->saved_stdout = -1;

    // Setup input redirection first
    if (cmd->infile && setup_input(cmd) == -1)
        return (-1);

    // Then setup output redirection
    if (cmd->outfile && setup_output(cmd) == -1)
    {
        // Restore stdin if output redirection fails
        if (cmd->saved_stdin != -1)
        {
            dup2(cmd->saved_stdin, STDIN_FILENO);
            close(cmd->saved_stdin);
        }
        return (-1);
    }

    return (0);
}
