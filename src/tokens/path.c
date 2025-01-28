/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:35:50 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/28 12:49:28 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*try_direct_path(char *cmd)
{
	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*try_path_combination(char *path, char *cmd)
{
	char	*temp;
	char	*full_path;
	char	*result;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK | X_OK) == 0)
	{
		result = ft_strdup(full_path);
		free(full_path);
		return (result);
	}
	free(full_path);
	return (NULL);
}

static char	*search_in_path(char **paths, char *cmd)
{
	int		i;
	char	*result;

	i = 0;
	while (paths[i])
	{
		result = try_path_combination(paths[i], cmd);
		if (result)
			return (free_and_return(paths, result));
		i++;
	}
	free_array(paths);
	return (NULL);
}

char	*build_path(char *cmd, char **env)
{
	t_path	path_struct;

	if (!cmd || !env)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (try_direct_path(cmd));
	path_struct.path_var = get_path(env);
	if (!path_struct.path_var)
		return (NULL);
	path_struct.paths = ft_split(path_struct.path_var, ':');
	if (!path_struct.paths)
		return (NULL);
	return (search_in_path(path_struct.paths, cmd));
}
