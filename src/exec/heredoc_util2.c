/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:47:53 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:49:57 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_heredoc_resources(int saved_stdin, t_heredoc *heredocs,
		int heredoc_count)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (heredocs)
	{
		cleanup_all_heredocs(heredocs, heredoc_count);
		free(heredocs);
	}
}

int	handle_init_error(int saved_stdin, t_heredoc *heredocs, int heredoc_count)
{
	if (g_exit_status == 130)
	{
		cleanup_heredoc_resources(saved_stdin, heredocs, heredoc_count);
		return (1);
	}
	return (1);
}
