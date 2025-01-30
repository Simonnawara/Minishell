/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:09:23 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/29 12:54:04 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_status(char *expanded_arg)
{
	printf("%s : command not found\n", expanded_arg);
	g_exit_status = 127;
	return (1);
}

static char	*expand_exit_status(char *processed_arg, int *stop_processing)
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

static int	handle_expansion(char **cmd, char **processed_arg, char **env,
		int i)
{
	char	*expanded_arg;
	int		stop_processing;

	stop_processing = 1;
	expanded_arg = expand_exit_status(*processed_arg, &stop_processing);
	if (stop_processing && i == 0)
	{
		handle_exit_status(expanded_arg);
		return (1);
	}
	if (expanded_arg)
	{
		*cmd = expanded_arg;
		return (0);
	}
	expanded_arg = expand_variables(*processed_arg, env);
	if (expanded_arg)
		*cmd = expanded_arg;
	return (0);
}

static int handle_home_expansion(char **cmd, char **res, int i, char **env)
{
    char    *home_value;
    char    *user;
    char    *full_path;

    home_value = get_env_value("HOME", env);
    if (!home_value)
    {
        user = get_env_value("USER", env);
        if (!user)
            return (1);
        home_value = ft_strjoin("/home/", user);
        free(user);
        if (!home_value)
            return (1);
    }
    if (strlen(res[i]) > 1)
        full_path = ft_strjoin(home_value, res[i] + 1);
    else
        full_path = ft_strdup(home_value);
    if (!full_path)
    {
        if (!get_env_value("HOME", env))
            free(home_value);
        return (1);
    }
    free(res[i]);
    res[i] = full_path;
    *cmd = ft_strdup(full_path);
    if (!*cmd)
    {
        if (!get_env_value("HOME", env))
            free(home_value);
        free(full_path);
        return (1);
    }
    if (!get_env_value("HOME", env))
        free(home_value);
    return (0);
}

static t_token	*process_quoted_token(char **res, int i, char quote_type,
		char **env, t_token_type prev_type)
{
	char		*cmd;
	char		*processed_arg;
	t_token		*new_token;
	t_token_type	type;
	int			total_quotes;

	total_quotes = count_quotes(res[i], quote_type);
	cmd = get_command(res[i], total_quotes, quote_type);
	if (!cmd)
		return (NULL);
	processed_arg = cmd;
	if (quote_type == 34 && ft_strchr(processed_arg, '$'))
		if (handle_expansion(&cmd, &processed_arg, env, i))
			return (NULL);
	type = classify_token_prev(cmd, env, prev_type);
	if (ft_strcmp(cmd, "|") == 0)
		type = T_WORD;
	new_token = create_token(cmd, type);
	if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
	{
		new_token->res = res;
		new_token->echo_counter = i;
	}
	free(cmd);
	return (new_token);
}

static t_token *process_unquoted_token(char **res, int i, char **env, t_token_type prev_type)
{
    char        *cmd;
    char        *processed_arg;
    t_token     *new_token;
    t_token_type    type;

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

static int    process_token(t_token **tokens, char **res, int i, char **env)
{
    char        quote_type;
    t_token     *new_token;
    t_token_type prev_type;

    prev_type = T_WORD;
    if (*tokens)
    {
        t_token *last = *tokens;
        while (last->next)
            last = last->next;
        prev_type = last->type;
    }
    quote_type = get_quote_type(res, i);
    if (quote_type && count_quotes(res[i], quote_type) % 2 == 0)
        new_token = process_quoted_token(res, i, quote_type, env, prev_type);
    else
        new_token = process_unquoted_token(res, i, env, prev_type);
    if (!new_token)
        return (1);
    add_token(tokens, new_token);
    if (!*tokens)
    {
        ft_putendl_fd("Error: Tokens list is NULL after add_token", 2);
        return (1);
    }
    if (check_pipe(new_token->type, res, i)
        || check_redirect(new_token->type, res, i))
    {
        if (*tokens)
            free_token_list(*tokens);
        return (1);
    }
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
	tokens = NULL;
	ft_memset(&exec, 0, sizeof(t_exec));
	exec.env = env;
	exec.compteur_pipe = 0;
	res = tokenize(prompt);
	if (!res)
		return (1);
	i = -1;
	while (res[++i])
	{
		if (process_token(&tokens, res, i, env))
			return (free_array(res), 1);
	}
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
		if (!verify_forbidden_tokens(prompt))
		{
			reset_signals();
			parse_prompt(prompt, new_env);
			setup_signals();
		}
		free(prompt);
	}
	clear_history();
	free_array(new_env);
	return (g_exit_status);
}

/*
Tout ce qui reste a gerer (d'apres des tests rapides / rudimentaires) (pas vraiment necessaire a gerer)

Minishell output:
> $USER'$PATH'
simna : command not found

Expected Output :
$USER'$PATH'
simna$PATH: command not found

ls Makefile > cat
Fonctionne correctement

ls Makefile > "cat"
cree un fichier cat, et un fichier "cat", et ecris dans cat
le ficher "cat" est pas sence exister.
Meme chose avec tous les mots apres une redirection entre ""

Nivea norme un peu chiant, on a juste une fonction aui prends 5 parametres, mais on peut creer une strucutre.

*/