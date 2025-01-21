/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/21 11:33:00 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

static	int		nb_args(char **args)
{
	int		size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

static char *get_env_value(char *var_name, char **env)
{
    int     i;
    int     var_len;
    //char    *env_var;

    if (!var_name || !env)
        return (NULL);

    var_len = ft_strlen(var_name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var_name, var_len) == 0 && env[i][var_len] == '=')
            return (env[i] + var_len + 1);
        i++;
    }
    return (NULL);
}

// Helper function to expand environment variables
static char *expand_variables(char *str, char **env)
{
    char *result;
    size_t total_len;
    size_t var_len;
    int i, j;
    char *var_name;
    char *var_value;

    if (!str || !*str)
        return (ft_strdup(""));

    // First pass: calculate required buffer size
    total_len = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && 
            str[i + 1] != '"' && str[i + 1] != '\'')
        {
            i++; // Skip $
            var_len = 0;
            while (str[i + var_len] && (ft_isalnum(str[i + var_len]) || 
                  str[i + var_len] == '_'))
                var_len++;
            
            // Extract variable name
            var_name = ft_substr(str, i, var_len);
            if (!var_name)
                return (NULL);
            
            // Get variable value
            var_value = get_env_value(var_name, env);
            total_len += var_value ? ft_strlen(var_value) : 0;
            
            free(var_name);
            i += var_len;
        }
        else
        {
            total_len++;
            i++;
        }
    }

    // Allocate buffer with exact required size
    result = malloc(sizeof(char) * (total_len + 1));
    if (!result)
        return (NULL);

    // Second pass: perform the actual expansion
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && 
            str[i + 1] != '"' && str[i + 1] != '\'')
        {
            i++; // Skip $
            var_len = 0;
            while (str[i + var_len] && (ft_isalnum(str[i + var_len]) || 
                  str[i + var_len] == '_'))
                var_len++;
            
            var_name = ft_substr(str, i, var_len);
            if (!var_name)
            {
                free(result);
                return (NULL);
            }
            
            var_value = get_env_value(var_name, env);
            if (var_value)
            {
                ft_strlcpy(result + j, var_value, total_len - j + 1);
                j += ft_strlen(var_value);
            }
            
            free(var_name);
            i += var_len;
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

/* int	ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
{
	int		i;
	int		n_option;
	int		total_quotes;
	char	quote_type;
	char	*processed_arg;
	char 	*expanded_arg;

	if (!args)
    	return (1);
	i = 1;
	n_option = 0;
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (res[echo_counter + i])
		{
			quote_type = 0;
			if (res[echo_counter + i][0] == res[echo_counter + i][ft_strlen(res[echo_counter + i]) - 1]
				&& (res[echo_counter + i][0] == 34 || res[echo_counter + i][0] == 39))
			{
				quote_type = res[echo_counter + i][0];
<<<<<<< HEAD
=======
				//printf("quote_type : %c\n", quote_type);
>>>>>>> origin/main
				total_quotes = count_quotes(res[echo_counter + i], quote_type);
				processed_arg = get_command(res[echo_counter + i], total_quotes, quote_type);
				if (quote_type == '"' && ft_strchr(processed_arg, '$'))
				{
					expanded_arg = expand_variables(processed_arg, exec->env);
					ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
					free(expanded_arg);
				}
				else
                	ft_putstr_fd(processed_arg ? processed_arg : "", 1);
				free(processed_arg);
			}
			else if (ft_strchr(args[i], '$')) // Unquoted argument with potential variables
			{
				expanded_arg = expand_variables(args[i], exec->env);
				ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
				free(expanded_arg);
			}
			else
				ft_putstr_fd(args[i], 1);
			if (res[echo_counter + i + 1])
				ft_putstr_fd(" ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
} */


int ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
{
    int     i;
    int     n_option;
    int     total_quotes;
    char    quote_type;
    char    *processed_arg;
    char    *expanded_arg;
    int     res_index;

    if (!args || !res)
        return (1);

    i = 1;
    res_index = echo_counter + i;
    n_option = 0;

    if (nb_args(args) > 1)
    {
        // Handle -n option
        while (args[i] && ft_strcmp(args[i], "-n") == 0)
        {
            n_option = 1;
            i++;
            res_index = echo_counter + i;
        }

        // Process arguments until we hit a redirection or end
        while (res[res_index] && res[res_index][0] != '>' && res[res_index][0] != '<')
        {
            quote_type = 0;
            
            // Handle quoted arguments
            if (res[res_index][0] == res[res_index][ft_strlen(res[res_index]) - 1] 
                && (res[res_index][0] == 34 || res[res_index][0] == 39))
            {
                quote_type = res[res_index][0];
                total_quotes = count_quotes(res[res_index], quote_type);
                processed_arg = get_command(res[res_index], total_quotes, quote_type);
                
                if (processed_arg)
                {
                    if (quote_type == '"' && ft_strchr(processed_arg, '$'))
                    {
                        expanded_arg = expand_variables(processed_arg, exec->env);
                        ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
                        free(expanded_arg);
                    }
                    else
                        ft_putstr_fd(processed_arg, 1);
                    
                    free(processed_arg);
                }
            }
            // Handle unquoted arguments with variables
            else if (ft_strchr(args[i], '$'))
            {
                expanded_arg = expand_variables(args[i], exec->env);
                ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
                free(expanded_arg);
            }
            // Handle regular arguments
            else if (args[i])
                ft_putstr_fd(args[i], 1);

            // Add space only if next token exists and isn't a redirection
            if (res[res_index + 1] && res[res_index + 1][0] != '>' && res[res_index + 1][0] != '<')
                ft_putstr_fd(" ", 1);

            i++;
            res_index = echo_counter + i;
        }
    }

    if (n_option == 0)
        write(1, "\n", 1);

    return (0);
}