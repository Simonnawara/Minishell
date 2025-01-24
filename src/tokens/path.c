/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:35:50 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/24 13:20:02 by sinawara         ###   ########.fr       */
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

char *build_path(char *cmd, char **env)
{
	t_path path_struct;
    char *result;

	if (!cmd || !env)
		return (NULL);
	
    // Handle absolute paths and current directory
    if (cmd[0] == '/' || cmd[0] == '.') {
        if (access(cmd, F_OK | X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }

    path_struct.path_var = get_path(env);
    if (!path_struct.path_var)
        return NULL;

    path_struct.paths = ft_split(path_struct.path_var, ':');
    if (!path_struct.paths)
        return NULL;

    for (int i = 0; path_struct.paths[i]; i++)
	{
        path_struct.temp = ft_strjoin(path_struct.paths[i], "/");
        if (!path_struct.temp)
		{
            free_array(path_struct.paths);
            return NULL;
        }

        path_struct.full_path = ft_strjoin(path_struct.temp, cmd);
        free(path_struct.temp);
        
        if (!path_struct.full_path) {
            free_array(path_struct.paths);
            return NULL;
        }

        if (access(path_struct.full_path, F_OK | X_OK) == 0) {
            result = ft_strdup(path_struct.full_path);
            free(path_struct.full_path);
            free_array(path_struct.paths);
            return result;
        }
        free(path_struct.full_path);
    }

    free_array(path_struct.paths);
    return NULL;
}
