/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:56:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/22 20:52:53 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_error(char **args)
{
    ft_putstr_fd("cd: ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(args[1], 2);
}

static char *get_env_value(char **env, const char *var)
{
    int i;
    int var_len;

    if (!env || !var)
        return (NULL);

    var_len = ft_strlen(var);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
        {
            char *value = ft_strdup(env[i] + var_len + 1);
            return value;
        }
        i++;
    }
    return (NULL);
}

static int update_env_var(t_exec *exec, const char *name, const char *value)
{
    char *var;
    char *args[3];
    int ret;

    if (!exec || !exec->env || !name || !value)
        return (1);

    var = ft_strjoin(name, "=");
    if (!var)
		return (1);


    args[0] = ft_strdup("export");
    if (!args[0])
    {
        free(var);
        return (1);
    }

    args[1] = ft_strjoin(var, value);
    free(var);
    if (!args[1])
    {
        free(args[0]);
        return (1);
    }

    args[2] = NULL;
    ret = ft_export(args, exec);
    free(args[0]);
    free(args[1]);
    return (ret);
}
static int update_pwd_vars(t_exec *exec, const char *new_path)
{
    char *old_pwd;
    int ret;

    if (!exec || !new_path)
        return (1);

    old_pwd = get_env_value(exec->env, "PWD");

    if (old_pwd)
    {
        ret = update_env_var(exec, "OLDPWD", old_pwd);
        free(old_pwd);
        if (ret != 0)
            return (ret);
    }

    return update_env_var(exec, "PWD", new_path);
}

static char *get_absolute_path(char *base, char *arg)
{
    char *result;

    if (!arg)
        return NULL;

    if (arg[0] == '/')
    {
        result = ft_strdup(arg);
        return result;
    }

    if (base[ft_strlen(base) - 1] == '/')
        result = ft_strjoin(base, arg);
    else
    {
        char *temp = ft_strjoin(base, "/");
        if (!temp)
            return NULL;
        result = ft_strjoin(temp, arg);
        free(temp);
    }

    return result;
}

int ft_cd(char **args, t_exec *exec)
{
    char *target;
    char *pwd;
    int ret;

    if (!exec || !exec->env)
        return (1);

    pwd = get_env_value(exec->env, "PWD");
    if (!pwd)
        pwd = ft_strdup("/");

    // Different cases for cd
    if (!args[1] || ft_strcmp(args[1], "~") == 0)  // cd or cd ~
    {
        target = get_env_value(exec->env, "HOME");
        if (!target)
        {
            free(pwd);
            ft_putendl_fd("cd: HOME not set", 2);
            return (1);
        }
    }
    else if (ft_strcmp(args[1], "-") == 0)  // cd -
    {
        target = get_env_value(exec->env, "OLDPWD");
        if (!target)
        {
            free(pwd);
            ft_putendl_fd("cd: OLDPWD not set", 2);
            return (1);
        }
        ft_putendl_fd(target, 1);
    }
    else  // cd path
    {
        target = get_absolute_path(pwd, args[1]);
        if (!target)
        {
            free(pwd);
            return (1);
        }
    }

    ret = chdir(target);

    if (ret == 0)
        ret = update_pwd_vars(exec, target);
    else
    {
        print_error(args);
        ret = 1;
    }

    free(pwd);
    free(target);
    return ret;
}