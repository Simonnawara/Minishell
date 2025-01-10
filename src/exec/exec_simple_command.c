/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:08:44 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/10 15:29:13 by sinawara         ###   ########.fr       */
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
	if (!strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	if (!strcmp(cmd->cmd, "env"))
	    return (ft_env(cmd->args, exec->env));
    if (!strcmp(cmd->cmd, "export"))	
		return (ft_export(cmd->args, exec->env));
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
		//ret = execute_extern_cmd(&cmd, exec);
	exec->last_status = ret;
	return (ret);
}
