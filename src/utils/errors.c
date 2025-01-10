/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/10 15:55:09 by sinawara         ###   ########.fr       */
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
	char 			*cmd;
	
	quote_type = 0;
	if (word[0] == word[ft_strlen(word) - 1] && (word[0] == 34 || word[0] == 39))
		quote_type = word[0];
	total_quotes = count_quotes(word, quote_type);
	cmd = word;
	if (quote_type && total_quotes % 2 == 0) //checks if we have an even number of quotes
	{
		cmd = get_command(word, total_quotes, quote_type);
		if (!cmd)
			free_word_and_return(word, 1);
	}
	type = classify_token(cmd, env);
	if (type == T_WORD && !is_builtin(cmd))
	{
		write(2, cmd, ft_strlen(cmd));
		ft_putendl_fd(" : command not found", 2);
		return (1);
	}
	return (0);
}
