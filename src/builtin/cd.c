/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:56:52 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/21 15:01:34 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_error(char **args)
{
    //printf("DEBUG: Printing error for arg: %s\n", args[1] ? args[1] : "NULL");
    ft_putstr_fd("cd: ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(args[1], 2);
}

static char *get_env_value(char **env, const char *var)
{
    int i;
    int var_len;
    
    //printf("DEBUG: Getting env value for: %s\n", var);
    if (!env || !var)
    {
        printf("DEBUG: get_env_value received NULL: env=%p, var=%p\n", (void*)env, (void*)var);
        return (NULL);
    }
        
    var_len = ft_strlen(var);
    i = 0;
    while (env[i])
    {
        //printf("DEBUG: Checking env[%d]: %s\n", i, env[i]);
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
        {
            char *value = ft_strdup(env[i] + var_len + 1);
            //printf("DEBUG: Found value: %s\n", value ? value : "NULL");
            return value;
        }
        i++;
    }
    //printf("DEBUG: Variable %s not found in environment\n", var);
    return (NULL);
}

static int update_env_var(t_exec *exec, const char *name, const char *value)
{
    char *var;
    char *args[3];
    int ret;

    //printf("DEBUG: Updating env var: %s=%s\n", name, value);
    if (!exec || !exec->env || !name || !value)
    {
        //printf("DEBUG: update_env_var received NULL: exec=%p, name=%p, value=%p\n", 
               //(void*)exec, (void*)name, (void*)value);
        return (1);
    }

    var = ft_strjoin(name, "=");
    if (!var)
    {
        //printf("DEBUG: Failed to create var string\n");
        return (1);
    }

    args[0] = ft_strdup("export");
    if (!args[0])
    {
        //printf("DEBUG: Failed to create args[0]\n");
        free(var);
        return (1);
    }

    args[1] = ft_strjoin(var, value);
    free(var);
    if (!args[1])
    {
        //printf("DEBUG: Failed to create args[1]\n");
        free(args[0]);
        return (1);
    }

    args[2] = NULL;
    //printf("DEBUG: Calling ft_export with: %s\n", args[1]);
    ret = ft_export(args, exec);
    //printf("DEBUG: ft_export returned: %d\n", ret);
    free(args[0]);
    free(args[1]);
    return (ret);
}
static int update_pwd_vars(t_exec *exec, const char *new_path)
{
    char *old_pwd;
    int ret;

    //printf("DEBUG: Updating PWD vars with new path: %s\n", new_path);
    if (!exec || !new_path)
    {
        printf("DEBUG: update_pwd_vars received NULL: exec=%p, new_path=%p\n", 
               (void*)exec, (void*)new_path);
        return (1);
    }

    old_pwd = get_env_value(exec->env, "PWD");
    //printf("DEBUG: Current PWD: %s\n", old_pwd ? old_pwd : "NULL");
    
    if (old_pwd)
    {
        //printf("DEBUG: Updating OLDPWD\n");
        ret = update_env_var(exec, "OLDPWD", old_pwd);
        free(old_pwd);
        if (ret != 0)
        {
            //printf("DEBUG: Failed to update OLDPWD\n");
            return (ret);
        }
    }

    //printf("DEBUG: Updating PWD\n");
    return update_env_var(exec, "PWD", new_path);
}

static char *get_absolute_path(char *base, char *arg)
{
    //printf("DEBUG: get_absolute_path base=%s, arg=%s\n", base, arg);
    char *result;
    
    if (!arg)
        return NULL;
        
    if (arg[0] == '/')
    {
        result = ft_strdup(arg);
        //printf("DEBUG: absolute path: %s\n", result);
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
    
    //printf("DEBUG: constructed path: %s\n", result);
    return result;
}

int ft_cd(char **args, t_exec *exec)
{
    //printf("\nDEBUG: ft_cd starting with args[1] = %s\n", args[1] ? args[1] : "NULL");
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
        //printf("DEBUG: cd to HOME\n");
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
        //printf("DEBUG: cd to OLDPWD\n");
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
        //printf("DEBUG: cd to path: %s\n", args[1]);
        target = get_absolute_path(pwd, args[1]);
        if (!target)
        {
            free(pwd);
            return (1);
        }
    }

    //printf("DEBUG: Attempting chdir to: %s\n", target);
    ret = chdir(target);
    
    if (ret == 0)
    {
        //printf("DEBUG: chdir successful\n");
        ret = update_pwd_vars(exec, target);
    }
    else
    {
        //printf("DEBUG: chdir failed, errno: %d\n", errno);
        print_error(args);
        ret = 1;
    }

    free(pwd);
    free(target);
    //printf("DEBUG: ft_cd returning: %d\n", ret);
    return ret;
}