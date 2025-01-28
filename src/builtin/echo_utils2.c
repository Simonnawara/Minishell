/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:43:19 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 19:05:21 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strip_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;
	char	outer_quote;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	outer_quote = 0;
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
		outer_quote = str[0];
	i = -1;
	j = 0;
	while (str[++i])
	{
		if ((str[i] != outer_quote) || (i != 0 && i != len - 1))
			result[j++] = str[i];
	}
	result[j] = '\0';
	return (result);
}

void	cleanup_echo_resources(char *stripped_arg, char *processed_arg,
		char *expanded_arg)
{
	if (stripped_arg)
		free(stripped_arg);
	if (processed_arg)
		free(processed_arg);
	if (expanded_arg)
		free(expanded_arg);
}

void	handle_expanded_arg(char *expanded_arg, char *stripped_arg,
		char *processed_arg)
{
	if (expanded_arg)
		ft_putstr_fd(expanded_arg, 1);
	else
		ft_putstr_fd("", 1);
	cleanup_echo_resources(NULL, processed_arg, expanded_arg);
	free(stripped_arg);
}

void	handle_processed_arg(char *processed_arg, char *stripped_arg)
{
	ft_putstr_fd(processed_arg, 1);
	cleanup_echo_resources(NULL, processed_arg, NULL);
	(void)stripped_arg;
}

void	handle_stripped_arg(char *stripped_arg)
{
	ft_putstr_fd(stripped_arg, 1);
	free(stripped_arg);
}
