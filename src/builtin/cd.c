/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:56:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/21 11:06:39 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static void print_error(char **args)
{
    ft_putstr_fd("cd: ", 2);
    if (args[2])
        ft_putstr_fd("string not in pwd: ", 2);
    else
    {
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd(": ", 2);
    }
    ft_putendl_fd(args[1], 2);
}

static char *get_env_path(char **env, const char *var, size_t len)
{
    int i;
    char *value;
    
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var, len) == 0)
        {
            value = ft_strdup(env[i] + len + 1);  // +1 to skip the '='
            return value;
        }
        i++;
    }
    return (NULL);
}

static int update_oldpwd(t_exec *exec)
{
    char cwd[PATH_MAX];
    char *oldpwd;
    
    if (getcwd(cwd, PATH_MAX) == NULL)
        return (1);
    oldpwd = ft_strjoin("OLDPWD=", cwd);
    if (!oldpwd)
        return (1);
        
    // Create args array for ft_export
    char *args[] = {oldpwd, NULL};
    ft_export(args, exec);
    
    free(oldpwd);
    return (0);
}

static int go_to_path(int option, t_exec *exec)
{
    int ret;
    char *env_path;

    env_path = NULL;
    if (option == 0)  // cd with no args - go to HOME
    {
        update_oldpwd(exec);
        env_path = get_env_path(exec->env, "HOME", 4);
        if (!env_path)
        {
            ft_putendl_fd("minishell: cd: HOME not set", 2);
            return (1);
        }
    }
    else if (option == 1)  // cd - (go to OLDPWD)
    {
        env_path = get_env_path(exec->env, "OLDPWD", 6);
        if (!env_path)
        {
            ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
            return (1);
        }
        update_oldpwd(exec);
    }
    
    ret = chdir(env_path);
    free(env_path);
    return (ret);
}

int ft_cd(char **args, t_exec *exec)
{
    int cd_ret;

    if (!args[1])
        return (go_to_path(0, exec));
        
    if (ft_strcmp(args[1], "-") == 0)
        cd_ret = go_to_path(1, exec);
    else
    {
        update_oldpwd(exec);
        cd_ret = chdir(args[1]);
        if (cd_ret < 0)
            cd_ret *= -1;
        if (cd_ret != 0)
            print_error(args);
    }
    return (cd_ret);
}