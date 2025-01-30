/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:43:35 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 10:39:53 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	get_last_token_type(t_token *tokens)
{
	t_token			*last;
	t_token_type	prev_type;

	prev_type = T_WORD;
	if (!tokens)
		return (prev_type);
	last = tokens;
	while (last->next)
		last = last->next;
	prev_type = last->type;
	return (prev_type);
}

char	*init_home_value(char **env)
{
	char	*home_value;
	char	*user;

	home_value = get_env_value("HOME", env);
	if (home_value)
		return (home_value);
	user = get_env_value("USER", env);
	if (!user)
		return (NULL);
	home_value = ft_strjoin("/home/", user);
	(void)user;
	free(user);
	return (home_value);
}

int	handle_empty_cmd(char **cmd, char *home_value, char *full_path, char **env)
{
	if (!*cmd)
	{
		if (!get_env_value("HOME", env))
			free(home_value);
		free(full_path);
		return (1);
	}
	return (0);
}
