/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   og_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:09:23 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/29 12:54:04 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_first_arg_exit_status(char *expanded_arg, int i)
{
	(void)i;
	printf("%s : command not found\n", expanded_arg);
	g_exit_status = 127;
	return (1);
}

char	*handle_exit_status_expansion(char *processed_arg, int *stop_processing)
{
	char	*expanded_arg;

	if (check_exit_status(processed_arg))
	{
		expanded_arg = check_and_replace_exit_status(processed_arg, g_exit_status);
		return (expanded_arg);
	}
	*stop_processing = 0;
	return (NULL);
}

char	*handle_double_quote_var_expansion(char *processed_arg, char **env)
{
	char	*expanded_arg;

	expanded_arg = expand_variables(processed_arg, env);
	return (expanded_arg);
}

int	handle_double_quote_expansion(char **cmd, char **processed_arg, 
									char **env, int i)
{
	char	*expanded_arg;
	int		stop_processing;

	stop_processing = 1;
	expanded_arg = handle_exit_status_expansion(*processed_arg, &stop_processing);
	if (stop_processing && i == 0)
	{
		handle_first_arg_exit_status(expanded_arg, i);
		return (1);
	}
	if (expanded_arg)
	{
		*cmd = expanded_arg;
		return (0);
	}
	expanded_arg = handle_double_quote_var_expansion(*processed_arg, env);
	*cmd = expanded_arg;
	free(*processed_arg);
	
	return (0);
}

int	handle_unquoted_first_arg_exit_status(char *expanded_arg)
{
	printf("%s : command not found\n", expanded_arg);
	g_exit_status = 127;
	return (1);
}

char	*handle_unquoted_exit_status_expansion(char *processed_arg, int *stop_processing)
{
	char	*expanded_arg;

	if (check_exit_status(processed_arg))
	{
		expanded_arg = check_and_replace_exit_status(processed_arg, g_exit_status);
		return (expanded_arg);
	}
	*stop_processing = 0;
	return (NULL);
}

char	*handle_unquoted_var_expansion(char *processed_arg, char **env)
{
	char	*expanded_arg;

	expanded_arg = expand_variables(processed_arg, env);
	return (expanded_arg);
}

int	handle_unquoted_expansion(char **cmd, char **processed_arg, 
								char **env, int i)
{
	char	*expanded_arg;
	int		stop_processing;

	stop_processing = 1;
	expanded_arg = handle_unquoted_exit_status_expansion(*processed_arg, &stop_processing);
	if (stop_processing && i == 0)
	{
		handle_unquoted_first_arg_exit_status(expanded_arg);
		return (1);
	}
	if (expanded_arg)
	{
		*cmd = expanded_arg;
		return (0);
	}
	expanded_arg = handle_unquoted_var_expansion(*processed_arg, env);
	*cmd = expanded_arg;
	free(expanded_arg);
	
	return (0);
}


// Process single token
int	parse_prompt(char *prompt, char **env)
{
	char **res;
	int i;
	int total_quotes;
	char quote_type;
	char *cmd;
	char *processed_arg;
	char *expanded_arg;
	t_token_type type;
	t_token *tokens;
	t_token *new_token;
	t_ast_node *ast;
	t_exec exec;
	t_token_type prev_type = T_WORD;

	if (!prompt || !*prompt)
		return (1);
	tokens = NULL;
	ft_memset(&exec, 0, sizeof(t_exec));
	exec.env = env;
	exec.compteur_pipe = 0;
	res = tokenize(prompt);
	if (!res)
		return (1);
	i = 0;
	while (res[i])
	{
		quote_type = get_quote_type(res, i);
		total_quotes = count_quotes(res[i], quote_type);
		if (quote_type && total_quotes % 2 == 0)
		{
			expanded_arg = NULL;
			cmd = get_command(res[i], total_quotes, quote_type);
			if (!cmd)
				return (free_prompt_resources(tokens, res, expanded_arg), 1);
			processed_arg = cmd;
			if (quote_type == 34 && ft_strchr(processed_arg, '$'))
				if (handle_double_quote_expansion(&cmd, &processed_arg, env, i))
        			break;
			type = classify_token_prev(cmd, env, prev_type);
			if (strcmp(cmd, "|") == 0)
				type = T_WORD;
			new_token = create_token(cmd, type);
			if (new_token && new_token->value
				&& !ft_strcmp(new_token->value, "echo"))
			{
				new_token->res = res;
				new_token->echo_counter = i;
			}
			free(cmd);
			if (!new_token)
				return (free_token_list_array(tokens, res), 1);
			add_token(&tokens, new_token);
			if (!tokens)
				ft_putendl_fd("Error: Tokens list is NULL after add_token",
					2);
		}
		else
		{
			cmd = res[i];
			processed_arg = cmd;
			if (quote_type == 0 && ft_strchr(processed_arg, '$'))
				if (handle_unquoted_expansion(&cmd, &processed_arg, env, i))
        			break;
			if (quote_type == 0 && res[i][0] == '~' && res[i][1] != '~')
			{
				char *home_value = get_env_value("HOME", env);
				if (!home_value)
				{
					char *user = get_env_value("USER", env);
					if (!user)
						return (1);
					home_value = ft_strjoin("/home/", user);
				}
				if (strlen(res[i]) > 1)
				{
					char *full_path = ft_strjoin(home_value, res[i] + 1);
					if (!full_path)
						return (1);
					cmd = full_path;
					free(res[i]);
					res[i] = full_path;
				}
				else
				{
					cmd = ft_strdup(home_value);
					if (!cmd)
						return (1);
					free(res[i]);
					res[i] = cmd;
				}
			}
			type = classify_token_prev(cmd, env, prev_type);
			new_token = create_token(cmd, type);
			if (new_token && new_token->value && !ft_strcmp(new_token->value,
					"echo"))
			{
				new_token->res = res;
				new_token->echo_counter = i;
			}
			if (!new_token)
				return (free_token_list_array(tokens, res), 1);
			add_token(&tokens, new_token);
			if (!tokens)
				ft_putendl_fd("Error: Tokens list is NULL after add_token", 2);
			if (check_pipe(new_token->type, res, i)
				|| check_redirect(new_token->type, res, i))
			{
				if (tokens)
					free_token_list(tokens);
				return (1);
			}
		}
		//print_token_info(new_token);
		if (is_command_found(res[0], env))
			return (0);
		prev_type = type;
		i++;
	}
	i = 0;
	while (res && res[i])
		i++;
	ast = build_ast(&tokens);
	if (!ast)
		return (free_token_list_array(tokens, res), 1);
	execute_ast(ast, &exec);
	if (ast)
	{
		free_ast_node(ast);
		ast = NULL;
	}
	free_array(res);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char *prompt;
	char **new_env;
	(void)argv;

	if (validate_inputs(argc))
		return (EXIT_FAILURE);
	using_history();
	new_env = init_env(env);
	setup_signals();
	while (1)
	{
		prompt = readline("> ");
		if (!prompt)
		{
			ft_printf("exit\n");
			break ;
		}
		if (ft_strlen(prompt) > 0)
			add_history(prompt);
		if (verify_forbidden_tokens(prompt))
		{
			free(prompt);
			continue ;
		}
		reset_signals();
		parse_prompt(prompt, new_env);
		setup_signals();
		free(prompt);
	}
	clear_history();
	free_array(new_env);
	return (g_exit_status);
}