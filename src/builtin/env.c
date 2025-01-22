/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:19:51 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/22 13:01:15 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void swap_strings(char **a, char **b)
{
    char *temp;
    
    temp = *a;
    *a = *b;
    *b = temp;
}

static int count_env_vars(char **env)
{
    int count;

    count = 0;
    while (env && env[count])
        count++;
    return (count);
}

static void sort_env(char **env)
{
    int i;
    int j;
    int len;

    len = count_env_vars(env);
    i = 0;
    while (i < len - 1)
    {
        j = 0;
        while (j < len - i - 1)
        {
            if (ft_strcmp(env[j], env[j + 1]) > 0)
                swap_strings(&env[j], &env[j + 1]);
            j++;
        }
        i++;
    }
}

static void update_underscore_var(char **env)
{
    int i;
    
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], "_=", 2) == 0)
        {
            free(env[i]);
            env[i] = ft_strdup("_=/usr/bin/env");
            break;
        }
        i++;
    }
}

int ft_env(t_exec *exec, char **args)
{
    int i;

    if (!exec || !exec->env)
        return (1);
    if (args && args[1])
    {
        ft_putendl_fd("env: too many arguments", 2);
        return (1);
    }

    // Update the underscore variable
    update_underscore_var(exec->env);
    
    // Sort the environment
    sort_env(exec->env);

    // Print sorted environment
    i = 0;
    while (exec->env[i])
    {
        ft_putendl_fd(exec->env[i], 1);
        i++;
    }

    return (0);
}