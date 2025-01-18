/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/18 10:56:14 by sinawara         ###   ########.fr       */
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
    char *var_start;
    char *var_name;
    char *var_value;
    int i;
    int j;

    if (!str || !*str)
        return (ft_strdup(""));

    // Allocate enough space for worst case (no expansions)
    result = malloc(sizeof(char) * (ft_strlen(str) * 2));
    if (!result)
        return (NULL);

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ' && str[i + 1] != '"' && str[i + 1] != '\'')
        {
            i++; // Skip the $
            var_start = &str[i];
            // Find end of variable name
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
            // Extract variable name
            var_name = ft_substr(var_start, 0, &str[i] - var_start);
            if (!var_name)
            {
                free(result);
                return (NULL);
            }
            // Look up variable in environment
            var_value = get_env_value(var_name, env);
            if (var_value)
            {
                // Copy variable value to result
                ft_strlcpy(&result[j], var_value, ft_strlen(var_value) + 1);
                j += ft_strlen(var_value);
            }
            free(var_name);
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

int	ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
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
				printf("quote_type : %c\n", quote_type);
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
}



/* int				ft_echo(char **args, char **res, int echo_counter)
{
	int		i;
	int		n_option;
	int		total_quotes;
	char	quote_type;
	char	*cmd;

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
				printf("quote_type : %c\n", quote_type);
				total_quotes = count_quotes(res[echo_counter + i], quote_type);
				cmd = get_command(res[echo_counter + i], total_quotes, quote_type);
				ft_putstr_fd(cmd, 1);
				free(cmd);
			}
			else
				ft_putstr_fd(args[i], 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
} */