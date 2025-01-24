/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_extern.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:08:41 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/24 11:12:22 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_child_error(char *cmd_path, char *cmd, int error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (error == EACCES)
		ft_putendl_fd(": Permission denied", 2);
	else if (error == ENOEXEC)
		ft_putendl_fd(": Exec format error", 2);
	else
		ft_putendl_fd(": Error executing command", 2);
	free(cmd_path);
	exit(126);
}

static void	execute_child_process(char *cmd_path, t_command_table *cmd,
		t_exec *exec)
{
	if (setup_redirection(cmd) == -1)
	{
		free(cmd_path);
		exit(1);
	}
	if (execve(cmd_path, cmd->args, exec->env) == -1)
		handle_child_error(cmd_path, cmd->cmd, errno);
}

static int	handle_parent_process(pid_t pid, char *cmd_path)
{
	int	status;

	free(cmd_path);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 131)
			ft_putendl_fd("Quit (core dumped)", 2);
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

int	execute_external_command(t_command_table *cmd, t_exec *exec)
{
	char	*cmd_path;
	pid_t	pid;

	if (!cmd->cmd || !*cmd->cmd)
		return (0);
	cmd_path = build_path(cmd->cmd, exec->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->cmd, 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (fork_error());
	}
	if (pid == 0)
		execute_child_process(cmd_path, cmd, exec);
	return (handle_parent_process(pid, cmd_path));
}
