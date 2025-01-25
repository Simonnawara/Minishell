/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/25 19:39:07 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

static int is_valid_n_option(const char *str)
{
    int i;

    if (!str || str[0] != '-' || str[1] != 'n')
        return (0);

    i = 1;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}


static char *get_env_value(char *var_name, char **env)
{
    int     i;
    int     var_len;

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
char *expand_variables(char *str, char **env)
{
    char *result;
    size_t total_len;
    size_t var_len;
    int i;
	int j;
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
            i++;
            var_len = 0;
            while (str[i + var_len] && (ft_isalnum(str[i + var_len]) ||
                  str[i + var_len] == '_'))
				  var_len++;

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
    result = malloc(sizeof(char) * (total_len + 1));
    if (!result)
	{
		free(var_name);
        return (NULL);
	}

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


static char *strip_quotes(const char *str)
{
    char    *result;
    int     i;
    int     j;
    int     len;

    if (!str)
        return (NULL);

    len = ft_strlen(str);
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return (NULL);

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] != '"' && str[i] != '\'')
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return (result);
}



static void cleanup_echo_resources(char *stripped_arg, char *processed_arg, char *expanded_arg)
{
    if (stripped_arg)
        free(stripped_arg);
    if (processed_arg)
        free(processed_arg);
    if (expanded_arg)
        free(expanded_arg);
}


int ft_echo(char **args, char **res, int echo_counter, t_exec *exec)
{
    int     i;
    int     n_option;
    char    *expanded_arg;
    int     res_index;
    int     prev_was_quote;
    char    *stripped_arg;
	int     total_quotes;
    char    quote_type;
    char    *processed_arg;

    if (!args || !res)
        return (1);

    i = 1;
    res_index = echo_counter + i;
    n_option = 0;
    prev_was_quote = 0;
    while (args[i] && is_valid_n_option(args[i])) // Handle -n option
	{
        n_option = 1;
        i++;
        res_index = echo_counter + i;
    }

    while (res[res_index] && res[res_index][0] != '>' && res[res_index][0] != '<')
    {
        stripped_arg = strip_quotes(res[res_index]);
        if (!stripped_arg)
		{
			cleanup_echo_resources(NULL, processed_arg, expanded_arg);
            return (1);
		}

		expanded_arg = NULL;
        processed_arg = NULL;

        if (ft_strchr(stripped_arg, '$'))
        {
			quote_type = 0;
			if (res[i][0] == res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34 || res[i][0] == 39))
				quote_type = res[res_index][0];
			total_quotes = count_quotes(res[res_index], quote_type);
			processed_arg = get_command(res[res_index], total_quotes, quote_type);
			if (!processed_arg)
			{
    			cleanup_echo_resources(stripped_arg, NULL, NULL);
				return (1);
			}

			if ((quote_type == '"' || quote_type == 0) && ft_strchr(processed_arg, '$'))
			{
				if (strcmp(processed_arg, "$?") == 0)
				{
					printf("%d\n", g_exit_status);
					//free(processed_arg);
					// if (!expanded_arg)
					// {
					cleanup_echo_resources(stripped_arg, processed_arg, NULL);
					// 	return (1);
					// }
					// ft_putstr_fd(expanded_arg, 1);
					//free(expanded_arg);
					return (0);
				}
				else
				{
					expanded_arg = expand_variables(processed_arg, exec->env);
					if (!expanded_arg)
					{
						cleanup_echo_resources(stripped_arg, processed_arg, NULL);
						return (1);
					}
					ft_putstr_fd(expanded_arg ? expanded_arg : "", 1);
					cleanup_echo_resources(NULL, processed_arg, expanded_arg);
				}
			}
			else
			{
				ft_putstr_fd(processed_arg, 1);
				cleanup_echo_resources(NULL, processed_arg, NULL);
          	}
        }
        else
		{
            ft_putstr_fd(stripped_arg, 1);
			free(stripped_arg);
		}

        int next_is_quote = (res[res_index + 1] &&
            (res[res_index + 1][0] == '"' || res[res_index + 1][0] == '\''));
        int curr_is_quote = (res[res_index][0] == '"' || res[res_index][0] == '\'');

        if (res[res_index + 1] &&
            res[res_index + 1][0] != '>' &&
            res[res_index + 1][0] != '<' &&
            !prev_was_quote &&
            !next_is_quote &&
            !curr_is_quote)
            ft_putstr_fd(" ", 1);

        prev_was_quote = curr_is_quote || next_is_quote;
        i++;
        res_index = echo_counter + i;
    }

    if (!n_option)
        write(1, "\n", 1);

    return (0);
}
