/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:05:34 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 14:30:37 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_last_status(t_exec *exec, int status)
{
	exec->last_status = status;
}

int	execute_pipe_child(t_ast_node *ast, t_exec *exec, int *pipe_fds,
		int is_left)
{
	int	status;

	if (is_left)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		status = execute_ast(ast->left, exec);
	}
	else
	{
		close(pipe_fds[1]);
		dup2(pipe_fds[0], STDIN_FILENO);
		close(pipe_fds[0]);
		status = execute_ast(ast->right, exec);
	}
	exit(status);
}

int	handle_pipe(t_ast_node *ast, t_exec *exec)
{
	int		pipe_fds[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status_left;
	int		status_right;

	if (pipe(pipe_fds) == -1)
		return (ft_putendl_fd("Error: Failed to create pipe", 2), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (ft_putendl_fd("Error: Fork failed", 2), 1);
	if (left_pid == 0)
		execute_pipe_child(ast, exec, pipe_fds, 1);
	right_pid = fork();
	if (right_pid == -1)
		return (kill(left_pid, SIGKILL), ft_putendl_fd("Error: Fork failed", 2),
			1);
	if (right_pid == 0)
		execute_pipe_child(ast, exec, pipe_fds, 0);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(left_pid, &status_left, 0);
	waitpid(right_pid, &status_right, 0);
	update_last_status(exec, WEXITSTATUS(status_right));
	return (WEXITSTATUS(status_right));
}

int	restore_io(t_command_table *cmd)
{
	if (cmd->saved_stdin != -1)
	{
		if (dup2(cmd->saved_stdin, STDIN_FILENO) == -1)
			return (ft_putendl_fd("Error: Failed to restore stdin", 2), -1);
		close(cmd->saved_stdin);
		cmd->saved_stdin = -1;
	}
	if (cmd->saved_stdout != -1)
	{
		if (dup2(cmd->saved_stdout, STDOUT_FILENO) == -1)
			return (ft_putendl_fd("Error: Failed to restore stdout", 2), -1);
		close(cmd->saved_stdout);
		cmd->saved_stdout = -1;
	}
	return (0);
}
