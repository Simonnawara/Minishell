/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:28:11 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:34:27 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	check_executable(char *token)
{
	struct stat	path_stat;

	if (access(token, F_OK) == 0)
	{
		if (stat(token, &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				return (T_WORD);
			if (access(token, X_OK) == 0)
				return (T_COMMAND);
		}
	}
	return (T_WORD);
}

int	check_consecutive_pipes(char **res)
{
	int	j;

	j = 0;
	while (res[j])
	{
		if (res[j][0] == '|')
		{
			j++;
			while (res[j] && ft_strlen(res[j]) == 0)
				j++;
			if (res[j] && res[j][0] == '|')
			{
				ft_putendl_fd(
					"minishell: syntax error near unexpected token `|'",
					2);
				g_exit_status = 2;
				return (1);
			}
			continue ;
		}
		j++;
	}
	return (0);
}

int	check_pipe_position(char **res, int i, t_token_type type)
{
	if (res[0][0] == '|')
	{
		ft_putendl_fd("Minishell : syntax error near unexpected token `|'", 2);
		g_exit_status = 2;
		return (1);
	}
	if (type == T_PIPE && res[i + 1] == NULL)
	{
		ft_putendl_fd("Error: Pipe at the end of the command", 2);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int	handle_redirect_error(char **res, int i)
{
	if (res[i + 1] == NULL)
	{
		ft_putendl_fd("Minishell: syntax error near unexpected token `newline'",
			2);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int	open_redirect_file(char **res, int i, int type)
{
	int	fd;

	if (type == T_REDIRECT_OUT)
		fd = open(res[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = open(res[i + 1], O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		file_error(res[i + 1]);
		return (1);
	}
	close(fd);
	return (0);
}
