/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:08:44 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 11:47:01 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", ft_strlen(cmd)) || 
		!ft_strncmp(cmd, "cd", ft_strlen(cmd)) ||
		!ft_strncmp(cmd, "pwd",ft_strlen(cmd)) || 
		!ft_strncmp(cmd, "export", ft_strlen(cmd)) ||
		!ft_strncmp(cmd, "unset", ft_strlen(cmd)) || 
		!ft_strncmp(cmd, "env", ft_strlen(cmd)) ||
		!ft_strncmp(cmd, "exit",ft_strlen(cmd)));
}

static int execute_builtin(t_command_table *cmd, t_exec *exec)
{
	(void)exec;
	if (!strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	return (0);
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
