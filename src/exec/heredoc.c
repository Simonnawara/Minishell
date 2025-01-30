/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 10:46:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:50:14 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*get_command_node(t_ast_node *ast)
{
	t_ast_node	*current;

	current = ast;
	while (current && current->type == T_HEREDOC)
		current = current->left;
	if (current && current->type != T_HEREDOC)
		return (current);
	return (NULL);
}

int	collect_heredocs(t_ast_node *ast, t_heredoc **hds)
{
	int			total;
	t_heredoc	*new_hds;
	int			setup_result;

	total = count_heredocs(ast);
	if (total == 0)
		return (0);
	new_hds = malloc(sizeof(t_heredoc) * total);
	if (!new_hds)
		return (1);
	setup_result = setup_heredoc_nodes(ast, new_hds, total);
	if (setup_result < 0)
		return (setup_result);
	*hds = new_hds;
	return (total);
}

void	cleanup_all_heredocs(t_heredoc *hds, int count)
{
	int	i;

	if (!hds)
		return ;
	i = 0;
	while (i < count)
	{
		if (hds[i].fd > 0)
		{
			close(hds[i].fd);
			hds[i].fd = -1;
		}
		if (hds[i].filename)
		{
			unlink(hds[i].filename);
			free(hds[i].filename);
			hds[i].filename = NULL;
		}
		i++;
	}
}

static int	init_heredoc(t_ast_node *ast, int *heredoc_count,
		t_heredoc **heredocs, int *saved_stdin)
{
	if (!ast || !ast->right || !ast->right->value)
	{
		g_exit_status = 2;
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	}
	*heredoc_count = collect_heredocs(ast, heredocs);
	if (*heredoc_count < 0)
		return (handle_heredoc_error(*heredocs, *heredoc_count));
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1 || *heredoc_count == 0)
		return (cleanup_all_heredocs(*heredocs, *heredoc_count), 1);
	return (0);
}

int	execute_heredoc(t_ast_node *ast, t_exec *exec)
{
	t_heredoc	*heredocs;
	int			heredoc_count;
	int			saved_stdin;
	int			last_heredoc_fd;
	int			status;

	heredocs = NULL;
	last_heredoc_fd = -1;
	if (init_heredoc(ast, &heredoc_count, &heredocs, &saved_stdin))
		return (handle_init_error(saved_stdin, heredocs, heredoc_count));
	if (heredoc_count > 0)
		last_heredoc_fd = heredocs[heredoc_count - 1].fd;
	if (g_exit_status != 130 && get_command_node(ast))
		status = execute_command_node(get_command_node(ast), exec,
				last_heredoc_fd);
	else
		status = 1;
	cleanup_heredoc_resources(saved_stdin, heredocs, heredoc_count);
	return (status);
}
