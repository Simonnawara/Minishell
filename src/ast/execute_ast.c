/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/16 11:21:37 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

int g_exit_status = 0;

static void	update_last_status(t_exec *exec, int status)
{
	exec->last_status = status;
}

static int	save_std_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
		return (ft_putendl_fd("Error: Failed to save stdin", 2), 1);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
	{
		close(*saved_stdin);
		return (ft_putendl_fd("Error: Failed to save stdout", 2), 1);
	}
	return (0);
}

static void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	handle_file_error(char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putendl_fd(": No such file or directory", 2);
	return (-1);
}

static int	setup_redirection2(t_ast_node *ast, int type)
{
	int	fd;

	if (type == T_REDIRECT_OUT)
		fd = open(ast->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == T_REDIRECT_IN)
		fd = open(ast->right->value, O_RDONLY);
	else if (type == T_APPEND)
		fd = open(ast->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		return (handle_file_error(ast->right->value));
	if (type == T_REDIRECT_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_redirection(t_ast_node *ast, t_exec *exec)
{
	int			saved_stdin;
	int			saved_stdout;
	int			status;
	t_ast_node	*current;

	if (save_std_fds(&saved_stdin, &saved_stdout))
		return (1);
	current = ast;
	while (current && current->type != T_COMMAND)
	{
		if (setup_redirection2(current, current->type) == -1)
		{
			restore_std_fds(saved_stdin, saved_stdout);
			return (1);
		}
		current = current->left;
	}
	if (current && current->type == T_COMMAND)
		status = execute_simple_command(current, exec);
	else
		status = 1;
	restore_std_fds(saved_stdin, saved_stdout);
	update_last_status(exec, status);
	return (status);
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
	if (!ast)
		return (0);
	if (ast->type == T_PIPE)
		return (handle_pipe(ast, exec));
	if (ast->type == T_REDIRECT_OUT || ast->type == T_REDIRECT_IN
		|| ast->type == T_APPEND)
		return (handle_redirection(ast, exec));
	if (ast->type == T_COMMAND)
		return (execute_simple_command(ast, exec));
	return (ft_putendl_fd("Error: Unknown node type", 2), 1);
}

// int execute_ast(t_ast_node *ast, t_exec *exec)
// {
	
//     int status;

//     if (!ast)
// 		return (0);
// 	if (exec->compteur_pipe > 0)
// 	{
		
// 		printf("We are about to enter pipe\n");
//         //init_pipe_nodes(ast, ast->args);
//        	status = execute_pipe_node(ast, exec);
//        	uptdate_last_status(exec, status);
// 		exec->compteur_pipe--;
// 		return (status);
// 	}
// //	printf("ast->type : %d\n", ast->type);
// //	printf("ast->value : %s\n", ast->value);

// 	//if (ast->type == T_PIPE)
//     //{
//         //printf("la\n");
//         //return (status);
//     //}

//     if (ast->type == T_COMMAND)
//     {
// 		//printf("AST VALUE : %s\n", ast->args[1]);
//         status = execute_simple_command(ast, exec);
//         uptdate_last_status(exec, status);
//         return (status);
//     }
//     return (1);
// }
