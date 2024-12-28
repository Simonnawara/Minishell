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


int parse_prompt(char *prompt, char **env)
{
	char **res;
	int i;
	int total_quotes;
	char quote_type;
	char *cmd;

	if (!ft_strncmp(prompt, "exit", 4))
		exit (EXIT_SUCCESS);
	res = tokenize(prompt);
	if (!res)
		return (ft_putendl_fd("Error: Tokenization failed", 2), 1);
	i = 0;
	while (res[i])
	{
		ft_printf("Token %d: %s\n", i + 1, res[i]); //prints the word to make sure we have it correctly

		if (res[i][0] ==  res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34 || res[i][0] == 39)) //means quotes qre around the word
			quote_type = res[i][0];
		total_quotes = count_quotes(res[i], quote_type); //calculates the amount of quotes in the word

		if (build_path(res[i], env))
			ft_printf("Is a command\n");
		else if (quote_type && total_quotes % 2 == 0) //checks if we have an even number of quotes
		{
			if (ft_strlen(res[i]) == 2)
			{
				printf("Word between quotes is empty\n");
				i++;
				continue;
			}
			cmd = get_command(res[i], total_quotes, quote_type);
			if (cmd && build_path(cmd, env))
				printf("Is between even quotes, and is a command\n");
			free(cmd);
		}
		else
			ft_printf("Is a standalone token or unknown\n");
		i++;
	}
	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (1);
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

/*
-> Parsing technique :  Precedence Climbing
						Generating an AST - Abstract Sytax Tree

What is grammar ?
Type of grammar used for this project = CFG (Context Free Grammar)
We can create a grammar that tells us how am arithmetic expression can be formed

Precedence Climbing :
https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing

Goal of the algo : treat an expression as a bunch of nested sub-expressions.
Each sub_expression has in common the lowest precedence level of the operators it contains.

- Get with the whole notion of Associativity

The precedence climbing algorithm is a recursice parsing technique often used for evaluating or parsing
mathematical expressions with operators of different precedence levels.
It ensures that higher precedence operators are evaluated before lower precedence ones

-> How it actually works :

- Atoms are either numbers or parenthesized expressions.
- Expressions consist of atoms connected by binary operators.

The algo is operator guided. Its fundamental step is to consume the next atom and look at the operator following it.
If the operator has precendence lower than the lowest acceptable for the current stop, the algorithm returns.
Otherwise, it calls itself in a loop to handle the sub-expression.


*/