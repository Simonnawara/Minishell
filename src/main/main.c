/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 10:36:46 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 10:36:46 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*process_quoted_token(char quote_type, char **env,
		t_token_type prev_type, t_quote_data *data)
{
	char			*cmd;
	char			*processed_arg;
	t_token			*new_token;
	t_token_type	type;
	int				total_quotes;

	total_quotes = count_quotes(data->res[data->i], quote_type);
	cmd = get_command(data->res[data->i], total_quotes, quote_type);
	if (!cmd)
		return (NULL);
	processed_arg = cmd;
	if (quote_type == 34 && ft_strchr(processed_arg, '$'))
		if (handle_expansion(&cmd, &processed_arg, env, data->i))
			return (NULL);
	type = classify_token_prev(cmd, env, prev_type);
	if (ft_strcmp(cmd, "|") == 0)
		type = T_WORD;
	new_token = create_token(cmd, type);
	if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
	{
		new_token->res = data->res;
		new_token->echo_counter = data->i;
	}
	free(cmd);
	return (new_token);
}

t_token	*process_unquoted_token(char **res, int i, char **env,
		t_token_type prev_type)
{
	char			*cmd;
	char			*processed_arg;
	t_token			*new_token;
	t_token_type	type;

	cmd = res[i];
	processed_arg = cmd;
	if (ft_strchr(processed_arg, '$'))
	{
		if (handle_expansion(&cmd, &processed_arg, env, i))
			return (NULL);
	}
	else if (res[i][0] == '~' && res[i][1] != '~')
	{
		if (handle_home_expansion(&cmd, res, i, env))
			return (NULL);
	}
	type = classify_token_prev(cmd, env, prev_type);
	new_token = create_token(cmd, type);
	if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
	{
		new_token->res = res;
		new_token->echo_counter = i;
	}
	return (new_token);
}

int	process_token(t_token **tokens, char **res, int i, char **env)
{
	char			quote_type;
	t_token			*new_token;
	t_token_type	prev_type;
	t_quote_data	*data;

	prev_type = get_last_token_type(*tokens);
	quote_type = get_quote_type(res, i);
	data = init_quote_data(res, i);
	if (!data)
		return (1);
	if (quote_type && count_quotes(res[i], quote_type) % 2 == 0)
		new_token = process_quoted_token(quote_type, env, prev_type, data);
	else
		new_token = process_unquoted_token(res, i, env, prev_type);
	free(data);
	if (!new_token)
		return (1);
	add_token(tokens, new_token);
	if (!*tokens)
		return (ft_putendl_fd("Error: Tokens list is NULL", 2), 1);
	if ((check_pipe(new_token->type, res, i) || check_redirect(new_token->type,
				res, i)) && *tokens && (free_token_list(*tokens), 1))
		return (1);
	return (0);
}

int	parse_prompt(char *prompt, char **env)
{
	char		**res;
	int			i;
	t_token		*tokens;
	t_ast_node	*ast;
	t_exec		exec;

	if (!prompt || !*prompt)
		return (1);
	init_parse_data(&exec, env, &tokens, &ast);
	res = tokenize(prompt);
	if (!res)
		return (1);
	i = -1;
	while (res[++i])
		if (process_token(&tokens, res, i, env))
			return (free_array(res), 1);
	if (is_command_found(res[0], env))
		return (0);
	ast = build_ast(&tokens);
	if (!ast)
		return (free_token_list_array(tokens, res), 1);
	execute_ast(ast, &exec);
	free_ast_node(ast);
	free_array(res);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*prompt;
	char	**new_env;

	(void)argv;
	if (validate_inputs(argc))
		return (EXIT_FAILURE);
	using_history();
	new_env = init_main(env);
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
		if (!verify_forbidden_tokens(prompt))
			security_signal(prompt, new_env);
		free(prompt);
	}
	clear_history();
	free_array(new_env);
	return (g_exit_status);
}
