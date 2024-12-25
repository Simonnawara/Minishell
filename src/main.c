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
	if (len <= quote_count)
	{
		ft_printf("word between quotes is too short\n");
		return (ft_strdup(""));
	}
	middle = malloc(sizeof(char) * (len - (quote_count) + 1));
	if (!middle)
		return (NULL);

	i = 0;
	j = 0;
	while (word[i])
	{
		while (word[i] == quote_type)
			i++;
		while(word[i] && word[i] != quote_type)
		{
			middle[j] = word[i];
			j++;
			i++;
		}
		break ;
	}
	middle[j] = '\0';
	return (middle);
}


int parse_prompt(char *prompt, char **env)
{
	char **res;
	int i = -1;
	int total_quotes;
	char quote_type;
	char *cmd;

	if (!ft_strncmp(prompt, "exit", 4))
		exit (EXIT_FAILURE);
	res = ft_split(prompt, ' ');
	while (res[++i])
	{
		if (res[i][0] == 34 || res[i][ft_strlen(res[i]) - 1] == 34) //determines of quote type is double
			quote_type = 34;
		else if (res[i][0] == 39 || res[i][ft_strlen(res[i]) - 1] == 39) // or single quotes
			quote_type = 39;
		else
			quote_type = 0;

		ft_printf("%s\n", res[i]); //prints the word to make sure we have it correct

		total_quotes = count_quotes(res[i], quote_type); //calculates the amount of quotes in the word
		if (build_path(res[i], env))
			ft_printf("Is a command\n");
		else if (quote_type && total_quotes % 2 == 0) //checks if we have an even number of quotes
		{
			cmd = get_command(res[i], total_quotes, quote_type);
			if (cmd && build_path(cmd, env))
				printf("Is between even quotes, and is a command\n");
			free(cmd);
		}
	}
	/* i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res); */
	return (1); // i = number of words in the prompt
}

int main(int argc, char **argv, char **env)
{
	(void)argv;
	if (validate_inputs(argc))
		return (EXIT_FAILURE);
	while (1)
	{
		char *prompt;
		prompt = readline("> ");
		if (!prompt)
		{
			ft_printf("Exiting...\n");
			break ;
		}
		parse_prompt(prompt, env);
		free(prompt);
	}
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
*/