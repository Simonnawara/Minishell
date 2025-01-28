/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 11:35:56 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	verify_forbidden_tokens(char *prompt)
{
	int		i;
	char	quote_type;

	i = -1;
	while (prompt[++i])
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
	}
	return (0);
}

// int	is_command_found(char *word, char **env)
// {
// 	t_token_type	type;
// 	int				total_quotes;
// 	char			quote_type;
// 	char			*processed_arg;
// 	char			*expanded_arg;

// 	if (!word || !env)
// 		return (0);
// 	quote_type = 0;
// 	if (word[0] == word[ft_strlen(word) - 1] && (word[0] == 34
// 			|| word[0] == 39))
// 		quote_type = word[0];
// 	total_quotes = count_quotes(word, quote_type);
// 	processed_arg = word;
// 	if (quote_type && total_quotes % 2 == 0)
// 	{
// 		processed_arg = get_command(word, total_quotes, quote_type);
// 		if (!processed_arg)
// 			free_word_and_return(word, 1);
// 	}
// 	type = classify_token(processed_arg, env);
// 	if (type == T_WORD && ft_strlen(processed_arg) != 0)
// 	{
// 		if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg,
// 				'$'))
// 		{
// 			printf("processed arg : %s\n", processed_arg);
// 			expanded_arg = expand_variables(processed_arg, env);
// 			if (expanded_arg)
// 			{
// 				write(2, expanded_arg, ft_strlen(expanded_arg));
// 				ft_putendl_fd(" : command not found", 2);
// 				g_exit_status = 127;
// 				free(expanded_arg);
// 			}
// 		}
// 		else
// 		{
// 			write(2, processed_arg, ft_strlen(processed_arg));
// 			ft_putendl_fd(" : command not found", 2);
// 			g_exit_status = 127;
// 			free(processed_arg);
// 		}
// 		return (1);
// 	}
// 	if (quote_type)
// 		free(processed_arg);
// 	return (0);
// }
static int	handle_expanded_command(char *processed_arg, char **env)
{
	char	*expanded_arg;

	expanded_arg = expand_variables(processed_arg, env);
	if (expanded_arg)
	{
		write(2, expanded_arg, ft_strlen(expanded_arg));
		ft_putendl_fd(" : command not found", 2);
		g_exit_status = 127;
		free(expanded_arg);
	}
	return (1);
}

static int	handle_command_not_found(char *processed_arg)
{
	write(2, processed_arg, ft_strlen(processed_arg));
	ft_putendl_fd(" : command not found", 2);
	g_exit_status = 127;
	free(processed_arg);
	return (1);
}

static char	*process_quoted_command(char *word, int total_quotes,
		char quote_type)
{
	char	*processed_arg;

	processed_arg = get_command(word, total_quotes, quote_type);
	if (!processed_arg)
		free_word_and_return(word, 1);
	return (processed_arg);
}

int	is_command_found(char *word, char **env)
{
	t_token_type	type;
	int				total_quotes;
	char			quote_type;
	char			*processed_arg;

	if (init_command_check(word, env, &quote_type))
		return (0);
	total_quotes = count_quotes(word, quote_type);
	processed_arg = word;
	if (quote_type && total_quotes % 2 == 0)
		processed_arg = process_quoted_command(word, total_quotes, quote_type);
	type = classify_token(processed_arg, env);
	if (type == T_WORD && ft_strlen(processed_arg) != 0)
	{
		if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg,
				'$'))
			return (handle_expanded_command(processed_arg, env));
		return (handle_command_not_found(processed_arg));
	}
	if (quote_type)
		free(processed_arg);
	return (0);
}
