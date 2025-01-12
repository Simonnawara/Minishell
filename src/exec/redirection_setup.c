/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:02:02 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/11 09:22:15 by trouilla         ###   ########.fr       */
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
