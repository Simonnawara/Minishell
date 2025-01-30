/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_extern.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:08:41 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 13:57:45 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_child_error(char *cmd_path, char *cmd, int error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (error == EACCES)
	{
		ft_putendl_fd(": Permission denied", 2);
		g_exit_status = 1;
	}
	else if (error == ENOEXEC)
	{
		ft_putendl_fd(": Exec format error", 2);
		g_exit_status = 1;
	}
	else
	{
		ft_putendl_fd(": Error executing command", 2);
		g_exit_status = 126;
	}
	free(cmd_path);
	exit(126);
}

/* static void	execute_child_process(char *cmd_path, t_command_table *cmd,
		t_exec *exec)
{
	if (setup_redirection(cmd) == -1)
	{
		free(cmd_path);
		g_exit_status = 1;
		exit(1);
	}
	if (execve(cmd_path, cmd->args, exec->env) == -1)
		handle_child_error(cmd_path, cmd->cmd, errno);
} */

static void execute_child_process(char *cmd_path, t_command_table *cmd,
    t_exec *exec)
{
    reset_signals();  // Reset signals to default in child
    if (setup_redirection(cmd) == -1)
    {
        free(cmd_path);
        g_exit_status = 1;
        exit(1);
    }
    if (execve(cmd_path, cmd->args, exec->env) == -1)
        handle_child_error(cmd_path, cmd->cmd, errno);
}

static int handle_parent_process(pid_t pid, char *cmd_path)
{
    int status;
    
    ignore_signals();  // Ignore signals while waiting
    free(cmd_path);
    waitpid(pid, &status, 0);
    setup_signals();   // Restore normal signal handlers
    
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 18);
            g_exit_status = 131;
            return (131);
        }
        else if (WTERMSIG(status) == SIGINT)
        {
            write(STDERR_FILENO, "\n", 1);
            g_exit_status = 130;
            return (130);
        }
        g_exit_status = 128 + WTERMSIG(status);
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
		g_exit_status = 127;
		ft_putendl_fd(cmd->cmd, 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		g_exit_status = 1;
		return (fork_error());
	}
	if (pid == 0)
		execute_child_process(cmd_path, cmd, exec);
	return (handle_parent_process(pid, cmd_path));
}
