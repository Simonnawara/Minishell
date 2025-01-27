/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/27 15:40:29 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	file_error(char *filename)
{
	write(2, filename, ft_strlen(filename));
	perror(" ");
}

int	verify_forbidden_tokens(char *prompt)
{
	int		i;
	char	quote_type;

	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '"' || prompt[i] == '\'')
		{
			quote_type = prompt[i];
			if (!move_past_quotes(prompt, quote_type, &i))
			{
				ft_putendl_fd("Error : Unclosed quote", 2);
				g_exit_status = 2;
				return (-1);
			}
			continue ;
		}
		if (prompt[i] == ';' || prompt[i] == '(' || prompt[i] == ')')
		{
			ft_putendl_fd("Error : Unauthorized token(s)", 2);
			g_exit_status = 2;
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_command_found(char *word, char **env)
{
	t_token_type	type;
	int				total_quotes;
	char			quote_type;
	char			*processed_arg;
	char			*expanded_arg;

	if (!word || !env)
		return (0);
	quote_type = 0;
	if (word[0] == word[ft_strlen(word) - 1] && (word[0] == 34
			|| word[0] == 39))
		quote_type = word[0];
	total_quotes = count_quotes(word, quote_type);
	processed_arg = word;
	if (quote_type && total_quotes % 2 == 0)
	{
		processed_arg = get_command(word, total_quotes, quote_type);
		if (!processed_arg)
			free_word_and_return(word, 1);
	}
	type = classify_token(processed_arg, env);
	if (type == T_WORD && ft_strlen(processed_arg) != 0)
	{
		if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg,
				'$'))
		{
			expanded_arg = expand_variables(processed_arg, env);
			if (expanded_arg)
			{
				write(2, expanded_arg, ft_strlen(expanded_arg));
				ft_putendl_fd(" : command not found", 2);
				g_exit_status = 127;
				free(expanded_arg);
			}
		}
		else
		{
			write(2, processed_arg, ft_strlen(processed_arg));
			ft_putendl_fd(" : command not found", 2);
			g_exit_status = 127;
			free(processed_arg);
		}
		return (1);
	}
	if (quote_type)
		free(processed_arg);
	return (0);
}


char    *check_and_replace_exit_status(const char *str, int exit_status)
{
    char    *dollar_pos;
    char    *result;
    char    *exit_str;
    int     len;

    // First check if $? exists
    if (!check_exit_status(str))
        return (strdup(str));

    // Find position of $?
    dollar_pos = strstr(str, "$?");
    
    // Convert exit status to string
    exit_str = ft_itoa(exit_status);
    if (!exit_str)
        return (NULL);
    
    // Calculate new length
    len = strlen(str) - 2 + strlen(exit_str);
    
    // Allocate memory for result
    result = (char *)malloc(sizeof(char) * (len + 1));
    if (!result)
    {
        free(exit_str);
        return (NULL);
    }
    
    // Copy parts before $?
    strncpy(result, str, dollar_pos - str);
    result[dollar_pos - str] = '\0';
    
    // Add exit status
    strcat(result, exit_str);
    
    // Add remaining string after $?
    strcat(result, dollar_pos + 2);
    
    free(exit_str);
    return (result);
}
