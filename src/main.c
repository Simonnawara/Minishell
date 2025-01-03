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
		if (word[i] == quote_type) //while(word[i] == quote_type)
			i++;
		else //while(word[i] && word[i] != quote_type)
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

	if (!prompt)
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
		ft_printf("Token %d: %s\n", i + 1, res[i]); //prints the word to make sure we have it correctly

		quote_type = 0;
		if (res[i][0] == res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34 || res[i][0] == 39))
			quote_type = res[i][0];
		total_quotes = count_quotes(res[i], quote_type); //calculates the amount of quotes in the word

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
				print_token_info(new_token);
				if (type == T_COMMAND)
					printf("Is between even quotes, and is a command\n");
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
			print_token_info(new_token);
			if (type == T_COMMAND)
				printf("Is a command\n");
			else if (type != T_WORD)
				ft_printf("Is an operator\n");
			else
				ft_printf("Is a standalone token or unknown\n");
		}
		i++;
	}

	printf("AST has not been built yet\n");
	ast = build_ast(&tokens);
	if (!ast)
	{
		free_token_list(tokens);
		return (1);
	}

	printf("AST built successfully\n");
	execute_ast(ast, env);
	printf("Executing succesfull\n\n");

	free(ast);
	free_token_list(tokens);
	free_array(res);

	return (0);
	//return (tokens);
}

/* int	parse_prompt(char *prompt, char **env)
{
	t_token		*tokens;
	t_ast_node	*ast;

	if (!ft_strncmp(prompt, "exit", 4))
		exit(EXIT_SUCCESS);
	tokens = tokenize_input(prompt, env);
	if (!tokens)
		return (ft_putendl_fd("Error: Tokenization failed", 2), 1);
	ast = build_ast(&tokens);
	if (!ast)
		return (free_token_list(tokens), 1);
	execute_ast(ast, env);
	print_full_ast(ast);
	free_ast(ast);
	free_token_list(tokens);
	return (0);
} */


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
