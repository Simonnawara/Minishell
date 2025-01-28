/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:52:15 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 14:53:01 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**handle_tokenize_error(char **tokens, int token_idx)
{
	while (--token_idx >= 0)
		free(tokens[token_idx]);
	free(tokens);
	return (NULL);
}
