/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:05:48 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	get_operator_type(char *token)
{
	if (token[0] == '<' && token[1] != '<')
		return (T_REDIRECT_IN);
	if (token[0] == '<' && token[1] == '<')
		return (T_HEREDOC);
	if (token[0] == '>' && token[1] != '>')
		return (T_REDIRECT_OUT);
	if (token[0] == '>' && token[1] == '>')
		return (T_APPEND);
	if (token[0] == '|' && token[1] != '|')
		return (T_PIPE);
	if (token[0] == '$' && token[1] == '?')
		return (T_EXIT_STATUS);
	if (token[0] == '(')
		return (T_PAREN_L);
	if (token[0] == ')')
		return (T_PAREN_R);
	return (T_WORD);
}

/* t_token_type	classify_token(char *token, char **env)
{
	t_token_type	type;
	struct stat		path_stat;
	char			*path;

	if (!token || !*token)
		return (T_WORD);
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
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
	if (is_builtin(token))
		return (T_COMMAND);
	path = build_path(token, env);
	if (path)
	{
		free(path);
		return (T_COMMAND);
	}
	return (T_WORD);
}

t_token_type	classify_token_prev(char *token, char **env,
		t_token_type prev_type)
{
	t_token_type	type;
	t_token_type	op_type;
	struct stat		path_stat;
	char			*path;

	if (!token || !*token)
		return (T_WORD);
	if (prev_type == T_COMMAND || prev_type == T_BUILTIN)
	{
		op_type = get_operator_type(token);
		if (op_type != T_WORD)
			return (op_type);
		return (T_WORD);
	}
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
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
	if (is_builtin(token))
		return (T_COMMAND);
	path = build_path(token, env);
	if (path)
	{
		free(path);
		return (T_COMMAND);
	}
	return (T_WORD);
}

int	check_pipe(t_token_type type, char **res, int i)
{
	int	j;

	j = 0;
	while (res[j])
	{
		if (res[j][0] == '|')
		{
			j++;
			while (res[j] && strlen(res[j]) == 0)
				j++;
			if (res[j] && res[j][0] == '|')
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `|'",
					2);
				g_exit_status = 2;
				return (1);
			}
			continue ;
		}
		j++;
	}
	if (res[0][0] == '|')
	{
		ft_putendl_fd("Minishell : syntax error near unexpected token `|'", 2);
		g_exit_status = 2;
		return (1);
	}
	if (type == T_PIPE)
	{
		if (res[i + 1] == NULL || res[i + 1][0])
		{
			ft_putendl_fd("Error: Pipe at the end of the command", 2);
			g_exit_status = 2;
			return (1);
		}
	}
	return (0);
}

int	check_redirect(t_token_type type, char **res, int i)
{
	if (type == T_REDIRECT_OUT || type == T_APPEND || type == T_REDIRECT_IN)
	{
		if (res[i + 1] == NULL)
		{
			ft_putendl_fd("Minishell: syntax error near unexpected token `newline'",
				2);
			g_exit_status = 2;
			return (1);
		}
		if (type == T_REDIRECT_OUT)
		{
			if (open(res[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644) < 0)
			{
				file_error(res[i + 1]);
				return (1);
			}
		}
		if (type == T_APPEND)
		{
			if (open(res[i + 1], O_WRONLY | O_CREAT, 0644) < 0)
			{
				file_error(res[i + 1]);
				return (1);
			}
		}
	}
	return (0);
}
 */


static t_token_type	check_executable(char *token)
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

t_token_type	classify_token(char *token, char **env)
{
	t_token_type	type;
	char			*path;

	if (!token || !*token)
		return (T_WORD);
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
	type = check_executable(token);
	if (type == T_COMMAND)
		return (type);
	if (is_builtin(token))
		return (T_COMMAND);
	path = build_path(token, env);
	if (path)
	{
		free(path);
		return (T_COMMAND);
	}
	return (T_WORD);
}

t_token_type	classify_token_prev(char *token, char **env,
		t_token_type prev_type)
{
	t_token_type	type;

	if (!token || !*token)
		return (T_WORD);
	if (prev_type == T_COMMAND || prev_type == T_BUILTIN)
	{
		type = get_operator_type(token);
		if (type != T_WORD)
			return (type);
		return (T_WORD);
	}
	return (classify_token(token, env));
}

static int	check_consecutive_pipes(char **res)
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
				ft_putendl_fd("minishell: syntax error near unexpected token `|'",
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

static int	check_pipe_position(char **res, int i, t_token_type type)
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

int	check_pipe(t_token_type type, char **res, int i)
{
	if (check_consecutive_pipes(res))
		return (1);
	return (check_pipe_position(res, i, type));
}

static int	handle_redirect_error(char **res, int i)
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

static int	open_redirect_file(char **res, int i, int type)
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

int	check_redirect(t_token_type type, char **res, int i)
{
	if (type != T_REDIRECT_OUT && type != T_APPEND && type != T_REDIRECT_IN)
		return (0);
	if (handle_redirect_error(res, i))
		return (1);
	if (type == T_REDIRECT_OUT || type == T_APPEND)
		return (open_redirect_file(res, i, type));
	return (0);
}
