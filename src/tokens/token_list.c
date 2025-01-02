/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:53:21 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 16:38:46 by sinawara         ###   ########.fr       */
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
	if (token[0] == '|' && token[1] == '|')
		return (T_OR);
	if (token[0] == '&' && token[1] == '&')
		return (T_AND);
	if (token[0] == ';')
		return (T_SEMICOLON);
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
