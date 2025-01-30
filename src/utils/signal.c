/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:33:58 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:50:40 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_no_line(void)
{
	if (g_exit_status == 130)
	{
		setup_signals();
		return (-1);
	}
	ft_putstr_fd("\nwarning: here-document delimited by end-of-file\n", 2);
	setup_signals();
	return (0);
}

void	write_line_to_fd(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	handle_setup_error(t_heredoc *new_hds, int i)
{
	if (g_exit_status == 130)
	{
		cleanup_all_heredocs(new_hds, i);
		free(new_hds);
		return (-1);
	}
	return (handle_heredoc_error(new_hds, i));
}

int	setup_heredoc_nodes(t_ast_node *current, t_heredoc *new_hds, int total)
{
	int	i;

	i = 0;
	while (current && current->type == T_HEREDOC && i < total)
	{
		if (setup_heredoc(current->right->value, &new_hds[i]) != 0)
			return (handle_setup_error(new_hds, i));
		i++;
		current = current->left;
	}
	return (i);
}
