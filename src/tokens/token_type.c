/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/29 23:49:21 by sinawara         ###   ########.fr       */
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
	if (prev_type == T_COMMAND || prev_type == T_BUILTIN
		|| prev_type == T_REDIRECT_IN || prev_type == T_REDIRECT_OUT
		|| prev_type == T_APPEND || prev_type == T_HEREDOC)
	{
		type = get_operator_type(token);
		if (type != T_WORD)
			return (type);
		return (T_WORD);
	}
	return (classify_token(token, env));
}

int	check_pipe(t_token_type type, char **res, int i)
{
	if (check_consecutive_pipes(res))
		return (1);
	return (check_pipe_position(res, i, type));
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
