/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:08:44 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 10:36:40 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit"));
}

static int execute_builtin(t_command_table *cmd, t_exec *exec)
{
	if (!strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
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
	if (is_builtin(cmd.cdm))
		ret = execute_builtin(&cmd, exec);
	else
		ret = execute_extern_cmd(&cmd, exec);
	exec->last_status = ret;
	return (ret);
}