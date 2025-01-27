/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:45:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/27 14:40:05 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_valid_id(char *str)
{
    int i;
    int equal_pos;

    if (!str || !*str || ft_isdigit(*str))
        return (0);
    
    i = 0;
    equal_pos = -1;
    while (str[i])
    {
        if (str[i] == '=' || (str[i] == '+' && str[i + 1] == '='))
        {
            equal_pos = i;
            break;
			(void)equal_pos;
        }
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static char *get_var_name(char *var)
{
    int i;
    char *name;

    i = 0;
    while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
        i++;
    name = ft_substr(var, 0, i);
    return (name);
}

static char *get_var_value(char *var)
{
    int i;

    i = 0;
    while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
        i++;
    
    if (!var[i])
        return (ft_strdup(""));
    else if (var[i] == '+')
        return (ft_strdup(var + i + 2));  // Skip '+='
    else
        return (ft_strdup(var + i + 1));  // Skip '='
}

static char *find_env_var(char **env, char *name)
{
    int i;
    size_t name_len;

    if (!env || !name)
        return (NULL);

    name_len = ft_strlen(name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], name, name_len) == 0 && 
            (env[i][name_len] == '=' || env[i][name_len] == '\0'))
            return (env[i]);
        i++;
    }
    return (NULL);
}

static int is_append_operation(char *var)
{
    int i;

    i = 0;
    while (var[i] && var[i] != '=' && !(var[i] == '+' && var[i + 1] == '='))
        i++;
    return (var[i] == '+' && var[i + 1] == '=');
}

static char *create_new_value(char *name, char *old_value, char *new_value, int is_append)
{
    char *result;
    size_t total_len;

    if (is_append && old_value)
    {
        total_len = ft_strlen(name) + 1 + ft_strlen(old_value) + ft_strlen(new_value) + 1;
        result = malloc(total_len);
        if (!result)
            return (NULL);
        ft_strlcpy(result, name, total_len);
        ft_strlcat(result, "=", total_len);
        ft_strlcat(result, old_value, total_len);
        ft_strlcat(result, new_value, total_len);
    }
    else
    {
        total_len = ft_strlen(name) + 1 + ft_strlen(new_value) + 1;
        result = malloc(total_len);
        if (!result)
            return (NULL);
        ft_strlcpy(result, name, total_len);
        ft_strlcat(result, "=", total_len);
        ft_strlcat(result, new_value, total_len);
    }
    return (result);
}

static int update_env(char **env, char *var)
{
    char *name;
    char *new_value;
    char *old_var;
    char *final_value;
    int is_append;
    int i;

    if (!env || !var)
        return (1);

    name = get_var_name(var);
    if (!name)
        return (1);

    is_append = is_append_operation(var);
    new_value = get_var_value(var);
    if (!new_value)
    {
        free(name);
        return (1);
    }

    old_var = find_env_var(env, name);
    if (old_var)
    {
        // Variable exists, update it
        char *old_value = NULL;
        if (is_append)
        {
            char *equals = ft_strchr(old_var, '=');
            old_value = equals ? equals + 1 : "";
        }
        final_value = create_new_value(name, old_value, new_value, is_append);
        if (!final_value)
        {
            free(name);
            free(new_value);
            return (1);
        }

        i = 0;
        while (env[i])
        {
            if (ft_strncmp(env[i], name, ft_strlen(name)) == 0 &&
                (env[i][ft_strlen(name)] == '=' || env[i][ft_strlen(name)] == '\0'))
            {
                free(env[i]);
                env[i] = final_value;
                break;
            }
            i++;
        }
    }
    else
    {
        // Variable doesn't exist, create it
        final_value = create_new_value(name, NULL, new_value, 0);
        if (!final_value)
        {
            free(name);
            free(new_value);
            return (1);
        }

        i = 0;
        while (env[i])
            i++;
        env[i] = final_value;
        env[i + 1] = NULL;
    }

    free(name);
    free(new_value);
    return (0);
}

int ft_export(char **args, t_exec *exec)
{
    int i;
    int status;

    if (!exec || !exec->env || !args)
        return (1);

    if (!args[1])
    {
        i = 0;
        while (exec->env[i])
            ft_printf("declare -x %s\n", exec->env[i++]);
        return (0);
    }

    status = 0;
    i = 1;
    while (args[i])
    {
        if (!is_valid_id(args[i]))
        {
            ft_printf("export: '%s': not a valid identifier\n", args[i]);
            status = 1;
        }
        else if (update_env(exec->env, args[i]) != 0)
            status = 1;
        i++;
    }
    return (status);
}
// static int	is_valid_id(char *str)
// {
// 	int	i;

// 	if (!str || !*str || ft_isdigit(*str))
// 		return (0);
// 	i = 0;
// 	while (str[i] && str[i] != '=')
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }
// static int	is_heap_pointer(void *ptr)
// {
// 	unsigned long	addr;

// 	addr = (unsigned long)ptr;
// 	return (addr < 0x7fff00000000UL && addr > 0x1000UL);
// }

// static int handle_append_operator(char *var, int key_len, char *old_env_entry, char **new_value)
// {
//     char *value_to_append;
//     char *old_value;
//     char *result;
//     int old_value_len;
//     int value_to_append_len;
    
//     // Skip the key and += operator
//     value_to_append = var + key_len + 2;
//     value_to_append_len = ft_strlen(value_to_append);
    
//     // Get the old value part after the '='
//     old_value = ft_strchr(old_env_entry, '=');
//     if (old_value)
//     {
//         old_value++; // Skip the '=' character
//         old_value_len = ft_strlen(old_value);
//     }
//     else
//     {
//         old_value = "";
//         old_value_len = 0;
//     }
    
//     // Allocate space for: key + '=' + old_value + new_value + null terminator
//     result = malloc(key_len + 1 + old_value_len + value_to_append_len + 1);
//     if (!result)
//         return (1);
    
//     // First copy the key part
//     ft_memcpy(result, var, key_len);
//     // Add the equals sign
//     result[key_len] = '=';
//     // Copy old value if it exists
//     if (old_value_len > 0)
//         ft_memcpy(result + key_len + 1, old_value, old_value_len);
//     // Copy new value
//     ft_memcpy(result + key_len + 1 + old_value_len, value_to_append, value_to_append_len);
//     // Add null terminator
//     result[key_len + 1 + old_value_len + value_to_append_len] = '\0';
    
//     *new_value = result;
//     return (0);
// }

// static int update_env(char **env, char *var)
// {
//     int i;
//     char *key;
//     int key_len;
//     char *new_value;
//     char *old_value;
//     int is_append;

//     if (!env || !var)
//         return (1);

//     i = 0;
//     key_len = 0;
//     // Find the end of the key (stopping at '=' or '+')
//     while (var[key_len] && var[key_len] != '=' && 
//            !(var[key_len] == '+' && var[key_len + 1] == '='))
//         key_len++;

//     // Check if this is an append operation
//     is_append = (var[key_len] == '+' && var[key_len + 1] == '=');

//     key = ft_substr(var, 0, key_len);
//     if (!key)
//         return (1);

//     // Look for existing variable
//     while (env[i])
//     {
//         // Match exact key length to avoid partial matches
//         if (ft_strncmp(env[i], key, key_len) == 0 && 
//             (env[i][key_len] == '=' || env[i][key_len] == '\0'))
//         {
//             old_value = env[i];
            
//             if (is_append)
//             {
//                 // Handle append operation
//                 if (handle_append_operator(var, key_len, old_value, &new_value))
//                 {
//                     free(key);
//                     return (1);
//                 }
//             }
//             else
//             {
//                 new_value = ft_strdup(var);
//                 if (!new_value)
//                 {
//                     free(key);
//                     return (1);
//                 }
//             }

//             // Free old value before assigning new one
//             if (is_heap_pointer(old_value))
//                 free(old_value);
//             env[i] = new_value;
//             free(key);
//             return (0);
//         }
//         i++;
//     }

//     // Variable doesn't exist yet, create it
//     if (is_append)
//     {
//         // For append to non-existent variable, treat as if empty string
//         if (handle_append_operator(var, key_len, "", &new_value))
//         {
//             free(key);
//             return (1);
//         }
//     }
//     else
//     {
//         new_value = ft_strdup(var);
//         if (!new_value)
//         {
//             free(key);
//             return (1);
//         }
//     }
    
//     free(key);
//     env[i] = new_value;
//     env[i + 1] = NULL;
//     return (0);
// }


// static char	**realloc_env(char **env)
// {
// 	char	**new_env;
// 	int		size;
// 	int		i;

// 	size = 0;
// 	while (env[size])
// 	{
// 		size++;
// 	}
// 	if (!(new_env = (char **)malloc(sizeof(char *) * (size + 2))))
// 		return (NULL);
// 	i = 0;
// 	while (env[i])
// 	{
// 		if (!(new_env[i] = ft_strdup(env[i])))
// 		{
// 			while (--i >= 0)
// 				free(new_env[i]);
// 			free(new_env);
// 			return (NULL);
// 		}
// 		free(env[i]);
// 		i++;
// 	}
// 	new_env[i] = NULL;
// 	new_env[i + 1] = NULL;
// 	free(env);
// 	return (new_env);
// }

// static int	add_to_env(t_exec *exec, char *var)
// {
// 	int		env_size;
// 	char	**new_env;

// 	if (!exec || !exec->env)
// 		return (1);
// 	env_size = 0;
// 	while (exec->env[env_size])
// 		env_size++;
// 	if (env_size % 10 == 9)
// 	{
// 		new_env = realloc_env(exec->env);
// 		if (!new_env)
// 			return (1);
// 		exec->env = new_env;
// 	}
// 	return (update_env(exec->env, var));
// }

// int	ft_export(char **args, t_exec *exec)
// {
// 	int i;
// 	int status;

// 	if (!exec || !exec->env || !args)
// 		return (1);

// 	if (!args[1])
// 	{
// 		i = 0;
// 		while (exec->env[i])
// 			ft_printf("declare -x %s\n", exec->env[i++]);
// 		return (0);
// 	}

// 	status = 0;
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!is_valid_id(args[i]))
// 			status = 1;
// 		else if (add_to_env(exec, args[i]) != 0)
// 			status = 1;
// 		i++;
// 	}
// 	return (status);
// }