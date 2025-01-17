/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/17 14:07:40 by trouilla         ###   ########.fr       */
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
	struct stat path_stat;

	if (!token || !*token)
		return (T_WORD);
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
	if (is_builtin(token))
		return (T_COMMAND);
	if (build_path(token, env)) // for straightforward commands
		return (T_COMMAND);
	if (access(token, X_OK) == 0) // for absolute paths
	{
		if (stat(token, &path_stat) != 0) //get file info
		{
			perror("stat"); //Handle error in retrieving file info
			return (T_WORD);
		}
		if (S_ISDIR(path_stat.st_mode)) //check if it's a directory
			return (T_WORD); //printf("Error : '%s' is a directory\n", token);
		else if (access(token, X_OK) == 0) //check if it's an executable
			return (T_COMMAND);
	}
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
		} // check if we cam access thhe file / create it depending on the type of redirection.
	}
	return (0);
}
