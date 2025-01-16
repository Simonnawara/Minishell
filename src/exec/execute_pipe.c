/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:14:28 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/16 10:57:29 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int handle_child_left(int *pipe_fd, t_ast_node *node, t_exec *exec)
// {
// 	printf("We are in left child\n");
//     if (exec->pipe_read != -1)
//     {
//         if (dup2(exec->pipe_read, STDIN_FILENO) == -1)
//             exit(1);
//         close(exec->pipe_read);
//         exec->pipe_read = -1;
//     }
//     close(pipe_fd[0]);
//     if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
//     {
//         close(pipe_fd[1]);
//         exit(1);
//     }
//     close(pipe_fd[1]);
//     exit(execute_ast(node->left, exec));
// }

// static int handle_child_right(int *pipe_fd, t_ast_node *node, t_exec *exec)
// {
// 	printf("We are in right child\n");
//     close(pipe_fd[1]);
//     if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
//     {
//         close(pipe_fd[0]);
//         return (1);
//     }
//     close(pipe_fd[0]);
//     return (execute_ast(node->right, exec));
// }

// static int wait_child(pid_t left_pid)
// {
// 	printf("We are in wait child\n");
//     int status;
//     int left_status;
    
//     if (waitpid(left_pid, &left_status, 0) == -1)
//         return (1);
        
//     while (wait(&status) > 0)
//         continue;
        
//     if (WIFEXITED(left_status))
//         return (WEXITSTATUS(left_status));
//     else if (WIFSIGNALED(left_status))
//         return (128 + WTERMSIG(left_status));
//     return (1);
// }

// int execute_pipe_node(t_ast_node *node, t_exec *exec)
// {
//     int pipe_fd[2];
//     pid_t pid;
//     int status;
    
// 	if (!node->left)
// 		printf("Node left if empty\n");

// 	if (!node->right)
// 		printf("Node right is empty\n");
	
//     if (!node->left || !node->right)
// 	{
// 		printf("Death\n");	
//         return (1);
// 	}
        
//     if (pipe(pipe_fd) == -1)
//         return (pipe_error());
        
//     pid = fork();
//     if (pid == -1)
//     {
//         close_pipe(pipe_fd);
//         return (fork_error());
//     }
// 	printf("la");
//     printf("pid est = %d\n", pid);
//     if (pid == 0)
// 	{
// 		printf("brother\n");
//         handle_child_left(pipe_fd, node, exec);
// 	}
        
//     close(pipe_fd[1]);
//     status = handle_child_right(pipe_fd, node, exec);
//     close(pipe_fd[0]);
//     wait_child(pid);
    
//     return (status);
// }

// static int	save_std_fds(int *saved_stdin, int *saved_stdout)
// {
// 	*saved_stdin = dup(STDIN_FILENO);
// 	if (*saved_stdin == -1)
// 		return (ft_putendl_fd("Error: Failed to save stdin", 2), 1);
// 	*saved_stdout = dup(STDOUT_FILENO);
// 	if (*saved_stdout == -1)
// 	{
// 		close(*saved_stdin);
// 		return (ft_putendl_fd("Error: Failed to save stdout", 2), 1);
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

// static int	setup_redirection(t_ast_node *ast, int type)
// {
// 	int	fd;

// 	if (type == T_REDIRECT_OUT)
// 		fd = open(ast->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	else if (type == T_REDIRECT_IN)
// 		fd = open(ast->right->value, O_RDONLY);
// 	else if (type == T_APPEND)
// 		fd = open(ast->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		return (-1);
// 	if (fd == -1)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(ast->right->value, 2);
// 		ft_putendl_fd(": No such file or directory", 2);
// 		return (-1);
// 	}
// 	if (type == T_REDIRECT_IN)
// 		dup2(fd, STDIN_FILENO);
// 	else
// 		dup2(fd, STDOUT_FILENO);
// 	close(fd);
// 	return (0);
// }

// int	handle_redirection(t_ast_node *ast, t_exec *exec)
// {
// 	int		saved_stdin;
// 	int		saved_stdout;
// 	int		status;
// 	t_ast_node	*current;

// 	if (save_std_fds(&saved_stdin, &saved_stdout))
// 		return (1);
// 	current = ast;
// 	while (current && current->type != T_COMMAND)
// 	{
// 		if (setup_redirection(current, current->type) == -1)
// 		{
// 			restore_std_fds(saved_stdin, saved_stdout);
// 			return (1);
// 		}
// 		current = current->left;
// 	}
// 	if (current && current->type == T_COMMAND)
// 		status = execute_simple_command(current, exec);
// 	else
// 		status = 1;
// 	restore_std_fds(saved_stdin, saved_stdout);
// 	update_last_status(exec, status);
// 	return (status);
// }