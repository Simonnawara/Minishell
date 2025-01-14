/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:14:28 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/14 16:47:46 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_child_left(int *pipe_fd, t_ast_node *node, t_exec *exec)
{
	printf("We are in left child\n");
    if (exec->pipe_read != -1)
    {
        if (dup2(exec->pipe_read, STDIN_FILENO) == -1)
            exit(1);
        close(exec->pipe_read);
        exec->pipe_read = -1;
    }
    close(pipe_fd[0]);
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        close(pipe_fd[1]);
        exit(1);
    }
    close(pipe_fd[1]);
    exit(execute_ast(node->left, exec));
}

static int handle_child_right(int *pipe_fd, t_ast_node *node, t_exec *exec)
{
	printf("We are in right child\n");
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        close(pipe_fd[0]);
        return (1);
    }
    close(pipe_fd[0]);
    return (execute_ast(node->right, exec));
}

static int wait_child(pid_t left_pid)
{
	printf("We are in wait child\n");
    int status;
    int left_status;
    
    if (waitpid(left_pid, &left_status, 0) == -1)
        return (1);
        
    while (wait(&status) > 0)
        continue;
        
    if (WIFEXITED(left_status))
        return (WEXITSTATUS(left_status));
    else if (WIFSIGNALED(left_status))
        return (128 + WTERMSIG(left_status));
    return (1);
}

int execute_pipe_node(t_ast_node *node, t_exec *exec)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    
	if (!node->left)
		printf("Node left if empty\n");

	if (!node->right)
		printf("Node right is empty\n");
	
    if (!node->left || !node->right)
	{
		printf("Death\n");	
        return (1);
	}
        
    if (pipe(pipe_fd) == -1)
        return (pipe_error());
        
    pid = fork();
    if (pid == -1)
    {
        close_pipe(pipe_fd);
        return (fork_error());
    }
	printf("la");
    printf("pid est = %d\n", pid);
    if (pid == 0)
	{
		printf("brother\n");
        handle_child_left(pipe_fd, node, exec);
	}
        
    close(pipe_fd[1]);
    status = handle_child_right(pipe_fd, node, exec);
    close(pipe_fd[0]);
    wait_child(pid);
    
    return (status);
}
