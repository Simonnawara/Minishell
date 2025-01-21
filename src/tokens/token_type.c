/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/21 15:18:02 by trouilla         ###   ########.fr       */
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
	{
		printf("6\n");
		return (T_WORD);
	}
	type = get_operator_type(token);
	if (type != T_WORD)
	{
		printf("5\n");
		return (type);
	}
	printf("SADSADLKASLDJASLDKJASLDJLASKDJD\n");
	printf("path_stat.st_mode : %d\n", path_stat.st_mode);
	if (S_ISDIR(path_stat.st_mode)) //check if it's a directory
	{
		printf("KAAAAAAAAAAAAAAAAAAAAAAA\n");
		return (T_WORD);
	}
	if (is_builtin(token))
	{
		printf("4\n");
		return (T_COMMAND);
	}
	if (build_path(token, env))
	{

		printf("3\n");
		return (T_COMMAND);
	} // for straightforward commands
	if (access(token, X_OK) == 0) // for absolute paths
	{
		printf("2\n");
		if (stat(token, &path_stat) != 0) //get file info
		{
			printf("1\n");
			perror("stat"); //Handle error in retrieving file info
			return (T_WORD);
		}
		if (S_ISDIR(path_stat.st_mode)) //check if it's a directory
			return (T_WORD); //printf("Error : '%s' is a directory\n", token);
		else if (access(token, X_OK) == 0) //check if it's an executable
		{
			printf("ISDIR\n");
			return (T_COMMAND);
		}
	}
	return (T_WORD);
}

int check_pipe(t_token_type type, char **res, int i)
{
	int j;

	j = 0;
	while (res[j])
	{
		// Check if current token is a pipe
		if (res[j][0] == '|')
		{
			j++; // Move to next token
			// Skip any empty tokens or spaces
			while (res[j] && strlen(res[j]) == 0)
				j++;
			// Check if next non-empty token is also a pipe
			if (res[j] && res[j][0] == '|')
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
				return (1);
			}
			continue;
		}
		j++;
	}
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
