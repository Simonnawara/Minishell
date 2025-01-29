/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:05 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/29 16:41:34 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	init_command(t_ast_node *ast, t_command_table *cmd,
		t_ast_node **cmd_node)
{
	*cmd_node = ast;
	while (*cmd_node && (*cmd_node)->type != T_COMMAND && (*cmd_node)->left)
		*cmd_node = (*cmd_node)->left;
	if (!(*cmd_node) || (*cmd_node)->type != T_COMMAND)
		return (ft_putendl_fd("Error: No command found", 2), 1);
	cmd->cmd = (*cmd_node)->value;
	cmd->args = (*cmd_node)->args;
	return (0);
}

static int	process_redirection(t_ast_node *redir, t_command_table *cmd,
		t_ast_node *cmd_node)
{
	while (redir && redir != cmd_node)
	{
		if (!redir->right || !redir->right->value)
			return (ft_putendl_fd("Error: Invalid redirection syntax", 2), 1);
		if (redir->type == T_REDIRECT_OUT || redir->type == T_APPEND)
		{
			cmd->outfile = redir->right->value;
			cmd->append = (redir->type == T_APPEND);
		}
		else if (redir->type == T_REDIRECT_IN)
			cmd->infile = redir->right->value;
		redir = redir->left;
	}
	return (0);
}

static void	cleanup_outfiles(t_command_table *cmd)
{
	int	i;

	i = 0;
	if (cmd->all_outfiles)
	{
		while (i < cmd->num_outfiles)
		{
			free(cmd->all_outfiles[i]);
			i++;
		}
		free(cmd->all_outfiles);
	}
}

static int	execute_redirection(t_ast_node *ast, t_exec *exec)
{
	t_command_table	cmd;
	int				status;
	t_ast_node		*cmd_node;

	ft_memset(&cmd, 0, sizeof(t_command_table));
	if (init_command(ast, &cmd, &cmd_node))
		return (1);
	if (process_redirection(ast, &cmd, cmd_node))
		return (1);
	if (setup_redirection(&cmd) == -1)
		return (1);
	status = execute_simple_command(cmd_node, exec, cmd);
	restore_io(&cmd);
	cleanup_outfiles(&cmd);
	return (status);
}

int	execute_ast(t_ast_node *ast, t_exec *exec)
{
	int				status;
	t_command_table	cmd;

	memset(&cmd, 0, sizeof(t_command_table));
	cmd.cmd = ast->value;
	cmd.args = ast->args;
	cmd.append = 0;
	if (!ast)
		return (0);
	if (ast->type == T_PIPE)
		status = handle_pipe(ast, exec);
	else if (ast->type == T_REDIRECT_OUT || ast->type == T_REDIRECT_IN
		|| ast->type == T_APPEND)
		status = execute_redirection(ast, exec);
	else if (ast->type == T_HEREDOC)
	{
		if (!ast->right || !ast->right->value)
			return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
		status = execute_heredoc(ast, exec);
	}
	else if (ast->type == T_COMMAND)
		status = execute_simple_command(ast, exec, cmd);
	else
		status = 1;
	return (status);
}
