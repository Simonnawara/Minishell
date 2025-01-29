/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:59:02 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/29 14:40:13 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char get_quote_type(char **res, int i)
{
	char quote_type;

	quote_type = 0;
	if (res[i][0] == res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34
				|| res[i][0] == 39))
			quote_type = res[i][0];
	return (quote_type);
}
void free_token_list_array(t_token *tokens, char **array)
{
	free_token_list(tokens);
	free_array(array);
}
