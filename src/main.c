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

int validate_inputs(int argc)
{
	if (argc < 1)
		return (ft_putendl_fd("Error : argument number invalid", 2), 1);
	return (0);
}

// Extracts a word between quotes.
char *get_command(char *word, int quote_count, char quote_type) // quote count = start + end quotes
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
		if (word[i] == quote_type)
			i++;
		else
		{
			middle[j] = word[i];
			j++;
			i++;
		}
	}
	middle[j] = '\0';
	return (middle);
}


int parse_prompt(char *prompt, char **env) //t_token *parse_prompt(char *prompt, char **env)
{
	char			**res;
	int				i;
	int				total_quotes;
	char			quote_type;
	char			*cmd;
	t_token_type	type;
	t_token			*tokens;
	t_token			*new_token;
	t_ast_node		*ast;

	if (!prompt || !*prompt)
		return (1);
	if (!ft_strncmp(prompt, "exit", 4))
		exit (EXIT_SUCCESS);
	tokens = NULL;
	res = tokenize(prompt);
	if (!res)
		return (ft_putendl_fd("Error: Tokenization failed", 2), 1);
	i = 0;
	
	while (res[i])
	{
		//printf("Token %d: %s\n", i + 1, res[i]); //prints the word to make sure we have it correctly

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
				if (!cmd)
				{
					free_token_list(tokens);
					free_array(res);
			        return (1);
				}

				type = classify_token(cmd, env);
				new_token = create_token(cmd, type);
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
			type = classify_token(res[i], env);
			new_token = create_token(res[i], type);
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
			|| check_redirect(new_token->type, res, i)
			|| check_parenth(res))
			{
				if (tokens)
					free_token_list(tokens);
				if (res)
					free_array(res);
				return (1);
			}
		}
		print_token_info(new_token);
		if (is_command_found(res[0], env))
		return (0);
		i++;
	}

	ast = build_ast(&tokens);
	if (!ast)
	{
		ft_putendl_fd("Error: Failed to build AST", 2);
		free_token_list(tokens);
		return (1);
	}
	execute_ast(ast, env);
	//printf("\033[1;32mExecuting succesfull\033[0m\n\n");

	free(ast);
	free_token_list(tokens);

	free_array(res);

	return (0);
	//return (tokens);
}


int main(int argc, char **argv, char **env)
{
	char *prompt;
	(void)argv;

	if (validate_inputs(argc))
		return (EXIT_FAILURE);

	using_history(); //initializes history managment
	while (1)
	{
		prompt = readline("> ");
		if (!prompt)
		{
			ft_printf("Exiting...\n");
			break ;
		}
		if (ft_strlen(prompt) > 0)
			add_history(prompt); //adds the last written prompt to the history

		parse_prompt(prompt, env);
		free(prompt);
	}
	clear_history();
	return (0);
}
