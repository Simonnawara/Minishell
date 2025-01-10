/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:08:44 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 15:41:39 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || 
		!ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || 
		!ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || 
		!ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit"));
}

static int execute_builtin(t_command_table *cmd, t_exec *exec)
{
	if (!strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	if (!strcmp(cmd->cmd, "env"))
	    return (ft_env(cmd->args, exec->env));
    if (!strncmp(cmd->cmd, "export", 6))
		return (ft_export(cmd->args, exec->env));
	return (1);
}
static int execute_extern_cmd(t_command_table *cmd, t_exec *exec)
{
	char *cmd_path;
	pid_t pid;
	int status;

	cmd_path = get_path(exec->env);
	if (!cmd_path)
	{
		ft_putendl_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->cmd,2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
		return (fork_error());
	if (pid == 0)
	{
		setup_redirection(cmd);
		execve(cmd_path, cmd->args, exec->env); //Proteger le execve avec un big_free
		exit(126);
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
int execute_simple_command(t_ast_node *node, t_exec *exec)
{
	t_command_table cmd;
	int ret;
	
	if (!node || !node->value)
		return (1);
	ft_memset(&cmd, 0, sizeof(t_command_table));
	cmd.cmd = node->value;
	cmd.args = node->args;
	cmd.infile = NULL;
	cmd.outfile = NULL;
	cmd.append = 0;
	if (is_builtin(cmd.cmd))
		ret = execute_builtin(&cmd, exec);
	//else
		//et = execute_extern_cmd(&cmd, exec);
	exec->last_status = ret;
	return (ret);
}
