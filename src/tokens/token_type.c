/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/23 10:36:34 by sinawara         ###   ########.fr       */
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
		
	if (access(token, F_OK) == 0) // for absolute paths
	{
		  if (stat(token, &path_stat) == 0)
        {
            // Check if it's a directory
            if (S_ISDIR(path_stat.st_mode))
                return (T_WORD);
            
            // If it's not a directory and is executable, it's a command
            if (access(token, X_OK) == 0)
                return (T_COMMAND);
        }
	}
	
	if (is_builtin(token))
		return (T_COMMAND);
	if (build_path(token, env))
		return (T_COMMAND); // for straightforward commands
	return (T_WORD);
}


t_token_type	classify_token_prev(char *token, char **env, t_token_type prev_type)
{
	t_token_type	type;
	t_token_type 	op_type;
	struct stat path_stat;

	if (!token || !*token)
		return (T_WORD);

	 // If previous token was a command, this token is a word unless it's an operator
    if (prev_type == T_COMMAND || prev_type == T_BUILTIN)
    {
        // Check if it's an operator first
        op_type = get_operator_type(token);
        if (op_type != T_WORD)  // If it's an operator, return that type
            return op_type;
        return T_WORD;  // Otherwise, it's a word (argument to the command)
    }
	
	type = get_operator_type(token);
	if (type != T_WORD)
		return (type);
		
	if (access(token, F_OK) == 0) // for absolute paths
	{
		  if (stat(token, &path_stat) == 0)
        {
            // Check if it's a directory
            if (S_ISDIR(path_stat.st_mode))
                return (T_WORD);
            
            // If it's not a directory and is executable, it's a command
            if (access(token, X_OK) == 0)
                return (T_COMMAND);
        }
	}
	
	if (is_builtin(token))
		return (T_COMMAND);
	if (build_path(token, env))
		return (T_COMMAND); // for straightforward commands
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
	if (res[0][0] == '|')
	{
		ft_putendl_fd("Minishell : syntax error near unexpected token `|'", 2);
		return (1);
	}
	if (type == T_PIPE)
	{
		if (res[i + 1] == NULL)
		{
			ft_putendl_fd("Error: Pipe at the end of the command", 2);
			return (1);
		}
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
