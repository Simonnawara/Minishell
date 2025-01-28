/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:30:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 15:24:53 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*s;
	unsigned char	*d;

	if (!dest && !src)
		return (dest);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return (dest);
}

int	init_command_check(char *word, char **env, char *quote_type)
{
	if (!word || !env)
		return (1);
	*quote_type = 0;
	if (word[0] == word[ft_strlen(word) - 1] && (word[0] == 34
			|| word[0] == 39))
		*quote_type = word[0];
	return (0);
}

char	*check_and_replace_exit_status(const char *str, int exit_status)
{
	char	*dollar_pos;
	char	*result;
	char	*exit_str;
	int		len;

	if (!check_exit_status(str))
		return (strdup(str));
	dollar_pos = strstr(str, "$?");
	exit_str = ft_itoa(exit_status);
	if (!exit_str)
		return (NULL);
	len = strlen(str) - 2 + strlen(exit_str);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
	{
		free(exit_str);
		return (NULL);
	}
	strncpy(result, str, dollar_pos - str);
	result[dollar_pos - str] = '\0';
	strcat(result, exit_str);
	strcat(result, dollar_pos + 2);
	free(exit_str);
	return (result);
}
