/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/25 14:43:46 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	file_error(char *filename)
{
	write(2, filename, ft_strlen(filename));
	perror(" ");
}

int verify_forbidden_tokens(char *prompt)
{
	int i;
	char quote_type;

	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '"' || prompt[i] == '\'')
		{
			quote_type = prompt[i];
			if (!move_past_quotes(prompt, quote_type, &i))
			{
				ft_putendl_fd("Error : Unclosed quote", 2);
				return (-1);
			}
			continue;
		}
		if (prompt[i] == ';' || prompt[i] == '(' || prompt[i] == ')')
		{
			ft_putendl_fd("Error : Unauthorized token(s)", 2);
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
	char    		*processed_arg;
    char    		*expanded_arg;

	if (!word || !env)
		return (0);
	quote_type = 0;
	if (word[0] == word[ft_strlen(word) - 1] && (word[0] == 34 || word[0] == 39))
		quote_type = word[0];
	total_quotes = count_quotes(word, quote_type);
	
	processed_arg = word;
	
	if (quote_type && total_quotes % 2 == 0) //checks if we have an even number of quotes
	{
		processed_arg = get_command(word, total_quotes, quote_type);
		if (!processed_arg)
			free_word_and_return(word, 1);
	}
	type = classify_token(processed_arg, env);
	
	if (type == T_WORD && ft_strlen(processed_arg) != 0)
	{
		if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg, '$'))
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
