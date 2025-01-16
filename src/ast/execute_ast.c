/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/16 15:48:38 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

void	update_last_status(t_exec *exec, int status)
{
	exec->last_status = status;
}


static int	execute_pipe_child(t_ast_node *ast, t_exec *exec,
	int *pipe_fds, int is_left)
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

static int	handle_pipe(t_ast_node *ast, t_exec *exec)
{
	int		pipe_fds[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pipe_fds) == -1)
		return (ft_putendl_fd("Error: Failed to create pipe", 2), 1);
	left_pid = fork();
	if (left_pid == -1)
		return (ft_putendl_fd("Error: Fork failed", 2), 1);
	if (left_pid == 0)
		execute_pipe_child(ast, exec, pipe_fds, 1);
	right_pid = fork();
	if (right_pid == -1)
		return (kill(left_pid, SIGKILL),
			ft_putendl_fd("Error: Fork failed", 2), 1);
	if (right_pid == 0)
		execute_pipe_child(ast, exec, pipe_fds, 0);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	exec->compteur_pipe--;
	update_last_status(exec, WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}


int	execute_ast(t_ast_node *ast, t_exec *exec)
{
	t_command_table cmd;
	
	cmd.cmd = ast->value;
	cmd.args = ast->args;
	//printf("comd node->args : %s\n", cmd.args[1]);
	cmd.infile = NULL;
	cmd.outfile = NULL;
	cmd.append = 0;
	if (!ast)
		return (0);
	printf("ast->type = %d\n", ast->type);
	if (ast->type == T_PIPE)
	{
		printf("Ronge MOIIIIIIIIIIIIIIIIII\n");
		return (handle_pipe(ast, exec));
	}
	if (ast->type == T_REDIRECT_OUT || ast->type == T_REDIRECT_IN
		|| ast->type == T_APPEND)
		{
			printf("NON ESCORTE\n");
			return (setup_redirection(&cmd));
		}
	if (ast->type == T_COMMAND)
	{
		printf("pute pute pute\n");
		return (execute_simple_command(ast, exec, cmd));
	}
	return (ft_putendl_fd("Error: Unknown node type", 2), 1);
}
