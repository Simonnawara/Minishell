/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:54:45 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/23 13:26:52 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
extern int g_exit_status;

int validate_inputs(int argc)
{
	if (argc < 1)
		return (ft_putendl_fd("Error : argument number invalid", 2), 1);
	return (0);
}

// Extracts a word between quotes, I think it needs some more frees()
char * get_command(char *word, int quote_count, char quote_type)
{
	char *middle;
	int len;
	int i;
	int j;

	len = ft_strlen(word);
	if (len == 2 && word[0] == quote_type && word[1] == quote_type)
        return (ft_strdup(""));
	middle = malloc(sizeof(char) * (len - quote_count + 1));
	if (!middle)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] != quote_type)
			middle[j++] = word[i];
		i++;
	}
	middle[j] = '\0';
	return (middle);
}


int parse_prompt(char *prompt, char **env)
{
	char			**res;
	int				i;
	int				total_quotes;
	char			quote_type;
	char			*cmd;
	char    		*processed_arg;
    char    		*expanded_arg;
	t_token_type	type;
	t_token			*tokens;
	t_token			*new_token;
	t_ast_node		*ast;
	t_exec			exec;
	t_token_type prev_type = T_WORD;

	if (!prompt || !*prompt)
		return (1);
	tokens = NULL;
	ft_memset(&exec, 0, sizeof(t_exec));
	exec.env = env;
	exec.compteur_pipe = 0;
	res = tokenize(prompt);
	if (!res)
		return (ft_putendl_fd("Error: Tokenization failed", 2), 1);
	i = 0;

	while (res[i])
	{
		quote_type = 0;
		if (res[i][0] == res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34 || res[i][0] == 39))
			quote_type = res[i][0];
		total_quotes = count_quotes(res[i], quote_type);
		if (quote_type && total_quotes % 2 == 0) //checks if we have an even number of quotes
		{
			if (ft_strlen(res[i]) == 2)
				printf("Word between quotes is empty\n");
			else
			{
				cmd = get_command(res[i], total_quotes, quote_type);
				processed_arg = cmd;
				if (quote_type == 34 && ft_strchr(processed_arg, '$'))
				{
					expanded_arg = expand_variables(processed_arg, env);
					cmd = expanded_arg;
				}
				if (!cmd)
				{
					free_token_list(tokens);
					free_array(res);
			        return (1);
				}
				type = classify_token_prev(cmd, env, prev_type);
				new_token = create_token(cmd, type);
				if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
				{
					new_token->res = res;
					new_token->echo_counter = i;
				}
				if (type == T_PIPE)
					exec.compteur_pipe++;
				free(cmd);
				if (!new_token)
				{
					free_token_list(tokens);
					free_array(res);
			        return (1);
				}
				add_token(&tokens, new_token);
				if (!tokens)
    				ft_putendl_fd("Error: Tokens list is NULL after add_token", 2);
			}
		}
		else
		{
			//printf("On est dans le else\n");
			cmd = res[i];
			processed_arg = cmd;
			if (quote_type == 0 && ft_strchr(processed_arg, '$'))
			{
				expanded_arg = expand_variables(processed_arg, env);
				cmd = expanded_arg;
				//free(processed_arg);
			}
			type = classify_token_prev(cmd, env, prev_type);
			new_token = create_token(cmd, type);
			if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
			{
				new_token->res = res;
				new_token->echo_counter = i;
			}
			if (type == T_PIPE)
				exec.compteur_pipe++;
			if (!new_token)
			{
				free_token_list(tokens);
				free_array(res);
		        return (1);
			}
			add_token(&tokens, new_token);
			if (!tokens)
				ft_putendl_fd("Error: Tokens list is NULL after add_token", 2);
			if (check_pipe(new_token->type, res, i)
			 || check_redirect(new_token->type, res, i))
			{
				if (tokens)
					free_token_list(tokens);
				if (res)
					free_array(res);
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
	{
		free_token_list(tokens);
		free_array(res);
		return (1);
	}
	//printf("conmpteur pipe : %d\n", exec.compteur_pipe);
	execute_ast(ast, &exec);
	free_ast(ast);
	free_token_list(tokens);
	free_array(res);
	return (0);
}
char **init_env(char **original_env)
{
    int i, size = 0;
    char **new_env;

    // Count environment variables
    while (original_env[size])
        size++;

    // Allocate new environment array
    new_env = (char **)malloc(sizeof(char *) * (size + 1));
    if (!new_env)
        return NULL;

    // Copy each environment string
    for (i = 0; i < size; i++)
    {
        new_env[i] = ft_strdup(original_env[i]);
        if (!new_env[i])
        {
            // Cleanup on error
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return NULL;
        }
    }
    new_env[size] = NULL;

    return new_env;
}


int main(int argc, char **argv, char **env)
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
