/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 09:56:57 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/29 13:19:18 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_inputs(int argc)
{
	if (argc > 1)
		return (ft_putendl_fd("Error : argument number invalid", 2), 1);
	return (0);
}

void	free_prompt_resources(t_token *tokens, char **res, char *expanded_arg)
{
	free_token_list(tokens);
	free_array(res);
	if (expanded_arg)
		free(expanded_arg);
}


char	*get_command(char *word, int quote_count, char quote_type)
{
	char	*middle;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(word);
	if (len == 2 && word[0] == quote_type && word[1] == quote_type)
		return (ft_strdup(""));
	middle = malloc(sizeof(char) * (len - quote_count + 1));
	if (!middle)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] != quote_type)
			middle[j++] = word[i];
		i++;
	}
	middle[j] = '\0';
	return (middle);
}

int	check_exit_status(const char *str)
{
	char	*pos;

	pos = strstr(str, "$?");
	if (pos)
		return (1);
	return (0);
}

char	**init_env(char **original_env)
{
	int		i;
	int		size;
	char	**new_env;

	size = 0;
	i = 0;
	while (original_env[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	while (i < size)
	{
		new_env[i] = ft_strdup(original_env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[size] = NULL;
	return (new_env);
}
