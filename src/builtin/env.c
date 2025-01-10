/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:10:20 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/10 15:20:56 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Compare environment variable names
static int env_name_cmp(const char *env_var, const char *name)
{
    while (*name && *env_var && *env_var != '=' && *name == *env_var)
    {
        name++;
        env_var++;
    }
    return (*env_var == '=' && *name == '\0');
}

// Find environment variable by name
static char *find_env_var(char **env, const char *name)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (env_name_cmp(env[i], name))
            return (env[i]);
        i++;
    }
    return (NULL);
}

// Get environment variable value
char *get_env_value(char **env, const char *name)
{
    char *var;
    char *value;

    var = find_env_var(env, name);
    if (!var)
        return (NULL);
    value = ft_strchr(var, '=');
    if (!value)
        return (NULL);
    return (value + 1);
}

// Count environment variables
static int count_env_vars(char **env)
{
    int count;

    count = 0;
    while (env[count])
        count++;
    return (count);
}

// Add or update environment variable
static char **update_env(char **env, const char *new_var)
{
    char **new_env;
    int i;
    int env_size;
    char *var_name;
    char *equals;

    // Extract variable name for comparison
    equals = ft_strchr(new_var, '=');
    if (!equals)
        return (NULL);
    var_name = ft_substr(new_var, 0, equals - new_var);
    if (!var_name)
        return (NULL);

    // Count existing variables
    env_size = count_env_vars(env);

    // Allocate new environment array
    new_env = malloc(sizeof(char *) * (env_size + 2));
    if (!new_env)
    {
        free(var_name);
        return (NULL);
    }

    // Copy existing variables, replacing if name matches
    i = 0;
    int replaced = 0;
    while (env[i])
    {
        if (env_name_cmp(env[i], var_name))
        {
            new_env[i] = ft_strdup(new_var);
            replaced = 1;
        }
        else
            new_env[i] = ft_strdup(env[i]);
        
        if (!new_env[i])
        {
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            free(var_name);
            return (NULL);
        }
        i++;
    }

    // Add new variable if it wasn't a replacement
    if (!replaced)
    {
        new_env[i] = ft_strdup(new_var);
        if (!new_env[i])
        {
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            free(var_name);
            return (NULL);
        }
        i++;
    }
    new_env[i] = NULL;
    free(var_name);
    return (new_env);
}

// Built-in: env command
int ft_env(char **args, char **env)
{
    int i;
    if (args[1])
    {
        ft_putendl_fd("env: too many arguments", 2);
        return (1);
    }
    i = 0;
	printf("In env\n");
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
	printf("LASKDJASKLJDASKLDJ\n");
    return (0);
}

// Built-in: export command
int ft_export(char **args, char **env)
{
    int i;
    char **new_env;

    if (!args[1])
    {
        // Print sorted environment variables with "declare -x " prefix
        return (0);
    }

    i = 1;
    while (args[i])
    {
        if (!ft_strchr(args[i], '='))
        {
            i++;
            continue;
        }

        new_env = update_env(env, args[i]);
        if (!new_env)
        {
            ft_putendl_fd("export: memory allocation error", 2);
            return (1);
        }

        // Free old environment and update pointer
        free_array(env);
        env = new_env;
        i++;
    }
    return (0);
}
