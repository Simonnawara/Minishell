/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 12:59:58 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	verify_forbidden_tokens(char *prompt)
{
	int	i;
	int	double_quotes;
	int	single_quotes;

	double_quotes = count_quotes(prompt, '"');
	single_quotes = count_quotes(prompt, '\'');
	if (double_quotes % 2 != 0 || single_quotes % 2 != 0)
	{
		ft_putendl_fd("Error : Unclosed quote", 2);
		g_exit_status = 2;
		return (-1);
	}
	i = -1;
	while (prompt[++i])
	{
		if (prompt[i] == ';' || prompt[i] == '(' || prompt[i] == ')')
		{
			ft_putendl_fd("Error : Unauthorized token(s)", 2);
			g_exit_status = 2;
			return (1);
		}
	}
	return (0);
}

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
