/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:17:02 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/21 14:59:42 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"


int is_builtin(char *cmd)
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
	t_ast_node temp_node;

	if (!strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args, cmd->res, cmd->echo_counter, exec));
	if (!strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd->args, exec));
	if (!strcmp(cmd->cmd, "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd->args, exec));
	if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(exec, cmd->args));
	if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env(exec, cmd->args));
	if (!ft_strcmp(cmd->cmd, "exit"))
	{
		temp_node.value = cmd->cmd;
		temp_node.args = cmd->args;
		return (ft_exit(&temp_node, exec));
	}
	return (1);
}
int execute_simple_command(t_ast_node *node, t_exec *exec, t_command_table cmd)
{
	int ret;

	printf("DEBUG: Executing command: '%s'\n", node->value);
	if (node->args)
	{
		int i = 0;
		while (node->args[i])
		{
			printf("DEBUG: arg[%d]: '%s'\n", i, node->args[i]);
			i++;
		}
	}
	if (!node || !node->value)
		return (1);
	ft_memset(&cmd, 0, sizeof(t_command_table));
	cmd.cmd = node->value;
	cmd.args = node->args;

	cmd.echo_counter = node->echo_counter;
	cmd.res = node->res;

	//printf("comd node->args : %s\n", cmd.args[1]);
	cmd.infile = NULL;
	cmd.outfile = NULL;
	cmd.append = 0;
	if (is_builtin(cmd.cmd))
		ret = execute_builtin(&cmd, exec);
	else
		ret = execute_external_command(&cmd, exec);
	exec->last_status = ret;
	return (ret);
}
