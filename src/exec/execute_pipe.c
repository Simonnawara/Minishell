/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:14:28 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/09 14:05:59 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_child_left(int *pipe_fd, t_ast_node *node, t_exec *exec)
{
	if (exec->pipe_read != -1) //S'assure de mettre le file directeur in a Read_pipe
	{
		dup2(exec->pipe_read, STDIN_FILENO);
		close(exec->pipe_read);
	}
	close(pipe_fd[0]); //ferme le fd inutilise
	dup2(pipe_fd[1], STDOUT_FILENO); //On passe ce qui se trouve a gauche de la pipe en sorti pour s'en servir comme in apres
	close(pipe_fd[1]);//on ferme le fd apres utilisation
	exit(execute_ast(node->left, exec));//et on execute ce qui se trouve gauche de la pipe
}
static int handle_child_right(int *pipe_fd, t_ast_node *node, t_exec *exec)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return(execute_ast(node->right, exec));
}
static int wait_child(pid_t left_pid)
{
	int status;
	int left_status;

	waitpid(left_pid, &left_status, 0);
	while (wait(&status) > 0)
		;
	if (WIFEXITED(left_status))
		return (WEXITSTATUS(left_status));
	return (1);
}
int execute_pipe_node(t_ast_node *node, t_exec *exec)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    
	if (!node->left || !node->right)
		return (1);
    if (pipe(pipe_fd) == -1)
        return (pipe_error());
    pid = fork();
    if (pid == -1)
    {
        close_pipe(pipe_fd);
		return (fork_error());
    }
	if (pid == 0)
		handle_child_left(pipe_fd, node, exec);
    close(pipe_fd[1]);
    status = handle_child_right(pipe_fd, node, exec);
    close(pipe_fd[0]);
	wait_child(pid);
    return (status);
}
