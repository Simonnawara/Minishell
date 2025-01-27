/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:33:14 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/27 16:05:10 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	fork_error(void)
{
	char	*msg;
	int		exit_code;

	msg = strerror(errno);
	ft_putstr_fd("minishell: fork error: ", 2);
	ft_putendl_fd(msg, 2);
	g_exit_status = 1;
	if (errno == EAGAIN)
		exit_code = 11;
	else
		exit_code = 1;
	return (exit_code);
}
int	handle_heredoc_error(t_heredoc *new_hds, int i)
{
	while (i > 0)
	{
		i--;
		cleanup_heredoc(&new_hds[i]);
	}
	free(new_hds);
	return (1);
}

int	handle_heredoc_error_2(t_heredoc *heredocs, int heredoc_count)
{
	g_exit_status = 2;
	cleanup_all_heredocs(heredocs, heredoc_count);
	return (ft_putstr_fd("minishell: heredoc: error\n", 2), 1);
}

int	setup_input_redirection(t_command_table *cmd, t_ast_node *current,
		t_exec *exec)
{
	int	fd;

	while (current && current->type == T_REDIRECT_IN)
	{
		cmd->infile = current->right->value;
		current = current->left;
	}
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (current)
		return (execute_simple_command(current, exec, *cmd));
	return (0);
}

int	execute_command_node(t_ast_node *cmd_node, t_exec *exec,
		int last_heredoc_fd)
{
	t_ast_node		*current;
	t_command_table	cmd;

	ft_memset(&cmd, 0, sizeof(t_command_table));
	current = cmd_node;
	cmd.cmd = current->value;
	cmd.args = current->args;
	if (last_heredoc_fd != -1)
	{
		dup2(last_heredoc_fd, STDIN_FILENO);
		close(last_heredoc_fd);
	}
	return (setup_input_redirection(&cmd, current, exec));
}