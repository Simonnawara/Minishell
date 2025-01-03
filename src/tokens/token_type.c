/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/03 22:34:16 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	get_operator_type(char *token)
{
	if (token[0] == '<')
		return (T_REDIRECT_IN);
	if (token[0] == '>' && token[1] != '>')
		return (T_REDIRECT_OUT);
	if (token[0] == '>' && token[1] == '>')
		return (T_APPEND);
	if (token[0] == '|' && token[1] != '|')
		return (T_PIPE);
	if (token[0] == '&' && token[1] == '&')
		return (T_AND);
	if (token[0] == '|' && token[1] == '|')
		return (T_OR);
	if (token[0] == ';')
		return (T_SEMICOLON);
	if (token[0] == '(')
		return (T_PAREN_L);
	if (token[0] == ')')
		return (T_PAREN_R);
	return (T_WORD);
}

t_token_type	classify_token(char *token, char **env)
{
	t_token_type	type;

	if (!token || !*token)
		return (T_WORD);
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
	if (access(token, F_OK | X_OK) == 0 || build_path(token, env))
		return (T_COMMAND);
	return (T_WORD);
}

int check_pipe(t_token_type type, char **res, int i)
{
	if (type == T_PIPE)
	{
		if (res[i + 1] == NULL)
		{
			ft_putendl_fd("Error: Pipe at the end of the command", 2);
			return (1);
		}
	}
	if (res[0][0] == '|')
	{
		ft_putendl_fd("Minishell : syntax error near unexpected token `|'", 2);
		return (1);
	}
	return (0);
}

int check_redirect(t_token_type type, char **res, int i)
{
	if (type == T_REDIRECT_OUT || type == T_APPEND || type == T_REDIRECT_IN)
	{
		if (res[i + 1] == NULL)
		{
			ft_putendl_fd("Minishell: syntax error near unexpected token `newline'", 2); //redirect at the end of the command
			return (1);
		}

		if (type == T_REDIRECT_OUT || type == T_APPEND)
			open(res[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		// check if we cam access thhe file / create it depending on the type of redirection.
	}
	return (0);
}

int check_parenth(char **res)
{
	int i;
	int j;
	int l_parenth;
	int r_parenth;

	l_parenth = 0;
	r_parenth = 0;
	if (!res)
		return (0);
	i = 0;
	while (res[i])
	{
		j = 0;
		while(res[i][j])
		{
			if (res[i][j] == '(')
				l_parenth++;
			if (res[i][j] == ')')
				r_parenth++;
			j++;
		}
		i++;
	}
	if (l_parenth > r_parenth)
	{
		ft_putendl_fd("Minishell: unclosed parenthesis", 2);
		return (1);
	}
	else if (r_parenth > l_parenth)
	{
		ft_putendl_fd("Minishell: syntax error near unexpected token ')'", 2);
		return (1);
	}
	return (0);
}
