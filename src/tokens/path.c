/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:35:50 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 16:38:46 by sinawara         ###   ########.fr       */
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


char	*build_path(char *cmd, char **env)
{
	int		i;
	t_path	path_struct;

	path_struct.path_var = get_path(env);
	if (!path_struct.path_var)
		return (NULL);
	path_struct.paths = ft_split(path_struct.path_var, ':');
	if (!path_struct.paths)
		return (NULL);
	i = -1;
	while (path_struct.paths[++i])
	{
		path_struct.temp = ft_strjoin(path_struct.paths[i], "/");
		if (!path_struct.temp)
			return ((char *)free_and_return(path_struct.paths, NULL));
		path_struct.full_path = ft_strjoin(path_struct.temp, cmd);
		free(path_struct.temp);
		if (!path_struct.full_path)
			return ((char *)free_and_return(path_struct.paths, NULL));
		if (access(path_struct.full_path, F_OK | X_OK) == 0)
		{
			printf("Command Path : %s\n", path_struct.full_path);
			return ((char *)free_and_return(path_struct.paths,
					path_struct.full_path));
		}
		free(path_struct.full_path);
	}
	return (free_and_return(path_struct.paths, NULL));
}
