/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:45:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/21 15:04:03 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_valid_id(char *str)
{
    //printf("DEBUG: Entering is_valid_id with str: %s\n", str);
    int i;

    if (!str || !*str || ft_isdigit(*str))
    {
        //printf("DEBUG: is_valid_id early return 0: !str=%d, !*str=%d, ft_isdigit=%d\n", 
               //!str, str ? !*str : -1, str ? ft_isdigit(*str) : -1);
        return (0);
    }
    i = 0;
    while (str[i] && str[i] != '=')
    {
        //printf("DEBUG: is_valid_id checking char: %c\n", str[i]);
        if (!ft_isalnum(str[i]) && str[i] != '_')
        {
            //printf("DEBUG: is_valid_id found invalid char at position %d\n", i);
            return (0);
        }
        i++;
    }
    //printf("DEBUG: is_valid_id returning 1\n");
    return (1);
}
static int is_heap_pointer(void *ptr)
{
    // Stack addresses typically start with 0x7fff
    // Static/global addresses typically start with lower values
    // Heap addresses are typically in between
    unsigned long addr = (unsigned long)ptr;
    return (addr < 0x7fff00000000UL && addr > 0x1000UL);
}

static int update_env(char **env, char *var)
{
    //printf("\nDEBUG: Entering update_env with var: %s\n", var);
    int     i;
    char    *key;
    int     key_len;
    char    *new_value;
    char    *old_value;

    if (!env || !var)
    {
        //printf("DEBUG: NULL pointer check failed: env=%p, var=%p\n", (void*)env, (void*)var);
        return (1);
    }

    i = 0;
    key_len = 0;
    while (var[key_len] && var[key_len] != '=')
        key_len++;
    //printf("DEBUG: Calculated key_len = %d\n", key_len);

    key = ft_substr(var, 0, key_len);
    if (!key)
    {
        //printf("DEBUG: Failed to create key substring\n");
        return (1);
    }
    //printf("DEBUG: Successfully created key: %s\n", key);

    while (env[i])
    {
        //printf("DEBUG: Checking env[%d]: %s\n", i, env[i]);
        if (ft_strncmp(env[i], key, key_len) == 0 &&
            (env[i][key_len] == '=' || env[i][key_len] == '\0'))
        {
            //printf("DEBUG: Found matching entry at index %d\n", i);
            
            old_value = env[i];
            //printf("DEBUG: Stored old value pointer: %p\n", (void*)old_value);
            
            //printf("DEBUG: Attempting to create new value\n");
            new_value = ft_strdup(var);
            if (!new_value)
            {
                //printf("DEBUG: Failed to create new value\n");
                free(key);
                return (1);
            }
            //printf("DEBUG: Successfully created new value: %s at %p\n", new_value, (void*)new_value);
            
            env[i] = new_value;
            //printf("DEBUG: Updated env[%d] with new value\n", i);
            
            // Only free if it's a heap pointer
            if (is_heap_pointer(old_value))
            {
                //printf("DEBUG: Old value is on heap, freeing at %p\n", (void*)old_value);
                free(old_value);
                //printf("DEBUG: Successfully freed old value\n");
            }
            else
            {
               // printf("DEBUG: Old value is not on heap (at %p), skipping free\n", (void*)old_value);
            }
            
            free(key);
           // printf("DEBUG: Successfully completed update\n");
            return (0);
        }
        i++;
    }
    
    //printf("DEBUG: No matching entry found - adding new entry at index %d\n", i);
    
    new_value = ft_strdup(var);
    if (!new_value)
    {
        //printf("DEBUG: Failed to create new value for new entry\n");
        free(key);
        return (1);
    }
    //printf("DEBUG: Successfully created new value: %s\n", new_value);
    
    free(key);
    env[i] = new_value;
    env[i + 1] = NULL;
    
   // printf("DEBUG: Successfully added new entry\n");
    return (0);
}
static char **realloc_env(char **env)
{
    //printf("DEBUG: Entering realloc_env\n");
    char    **new_env;
    int     size;
    int     i;

    size = 0;
    while (env[size])
    {
        ///printf("DEBUG: realloc_env counting size: %d\n", size);
        size++;
    }
    //printf("DEBUG: realloc_env total size: %d\n", size);
    
    if (!(new_env = (char **)malloc(sizeof(char *) * (size + 2))))
    {
       // printf("DEBUG: realloc_env malloc failed\n");
        return (NULL);
    }
    
    i = 0;
    while (env[i])
    {
      //  printf("DEBUG: realloc_env copying index %d\n", i);
        if (!(new_env[i] = ft_strdup(env[i])))
        {
            //printf("DEBUG: realloc_env strdup failed at index %d\n", i);
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return (NULL);
        }
        //printf("DEBUG: realloc_env freeing old entry at index %d\n", i);
        free(env[i]);
        i++;
    }
    new_env[i] = NULL;
    new_env[i + 1] = NULL;
    free(env);
    //printf("DEBUG: realloc_env completed successfully\n");
    return (new_env);
}

static int add_to_env(t_exec *exec, char *var)
{
    //printf("DEBUG: Entering add_to_env with var: %s\n", var);
    int     env_size;
    char    **new_env;

    if (!exec || !exec->env)
    {
       // printf("DEBUG: add_to_env early return: !exec=%d, !exec->env=%d\n", 
              // !exec, exec ? !exec->env : -1);
        return (1);
    }
    
    env_size = 0;
    while (exec->env[env_size])
    {
       // printf("DEBUG: add_to_env counting size: %d\n", env_size);
        env_size++;
    }
   // printf("DEBUG: add_to_env total size: %d\n", env_size);
    
    if (env_size % 10 == 9)
    {
       // printf("DEBUG: add_to_env needs reallocation\n");
        new_env = realloc_env(exec->env);
        if (!new_env)
        {
           // printf("DEBUG: add_to_env reallocation failed\n");
            return (1);
        }
        exec->env = new_env;
        //printf("DEBUG: add_to_env reallocation successful\n");
    }
    return (update_env(exec->env, var));
}

int ft_export(char **args, t_exec *exec)
{
   // printf("DEBUG: Entering ft_export\n");
    int i;
    int status;

    if (!exec || !exec->env || !args)
    {
        //printf("DEBUG: ft_export early return: !exec=%d, !exec->env=%d, !args=%d\n", 
             //  !exec, exec ? !exec->env : -1, !args);
        return (1);
    }
    
    if (!args[1])
    {
       // printf("DEBUG: ft_export no arguments, printing env\n");
        i = 0;
        while (exec->env[i])
            ft_printf("declare -x %s\n", exec->env[i++]);
        return (0);
    }
    
    status = 0;
    i = 1;
    while (args[i])
    {
        //printf("DEBUG: ft_export processing arg[%d]: %s\n", i, args[i]);
        if (!is_valid_id(args[i]))
        {
            //ft_printf("minishell: export: `%s': not a valid identifier\n",
                //args[i]);
            status = 1;
        }
        else if (add_to_env(exec, args[i]) != 0)
        {
            //printf("DEBUG: ft_export add_to_env failed for arg[%d]\n", i);
            status = 1;
        }
        i++;
    }
   // printf("DEBUG: ft_export returning with status: %d\n", status);
    return (status);
}