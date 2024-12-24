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

char *get_command(char *word, int quote_count) // quote count = start + end quotes
{
	int len;
	int i;

	len = ft_strlen(word);
	if (len <= 2)
	{
		ft_printf("word between quotes is too short\n");
		ft_printf("Message has been sent from the get_word function\n");
		return (ft_strdup(""));
	}
	char *middle = malloc(sizeof(char) * ((len - quote_count) + 1));
	if (!middle)
		return (NULL);
	i = 0;
	while (i < len - quote_count)
	{
		middle[i] = word[i + (quote_count /  2)];
		i++;
	}
	middle[i] = '\0';
	printf("Just FIY, this is the command : %s\n", middle);
	return (middle);
}

int count_quotes(char *word, char quote)
{
	int i;
	int quote_count;

	i = 0;
	quote_count = 0;
	while (word[i])
	{
		if (word[i] == quote)
			quote_count++;
		i++;
	}
	if (quote_count % 2 == 0)
		return (quote_count);
	return (1);
}

int parse_prompt(char *prompt, char **env)
{
	char **res;
	int i = -1;

	if (!ft_strncmp(prompt, "exit", 4))
		exit (EXIT_FAILURE);
	res = ft_split(prompt, ' ');
	while (res[++i])
	{
		ft_printf("%s\n", res[i]);

		if (build_path(res[i], env))
			ft_printf("Is a command\n");

		if (count_quotes(res[i], res[i][0]) != 1 || count_quotes(res[i], res[i][ft_strlen(res[i]) - 1]) != 1) //res[i][0] corresponds to the type of quote the is present, since, for now, we don't need to distinguish between single or double quotes
			{
				if (build_path(get_command(res[i], count_quotes(res[i], res[i][0])), env))
				{
					printf("Is between quotes, and is a command\n");
				}
			}

		/* if ((res[i][0] == '"' && res[i][ft_strlen(res[i]) - 1] == '"')
		|| (res[i][0] == 39 && res[i][ft_strlen(res[i]) - 1] == 39))
			if (build_path(get_command(res[i]), env)) */

	}
	return (i); // i = number of words in the prompt
}

int main(int argc, char **argv, char **env)
{
	(void)argv;
	validate_inputs(argc);
	while (1)
	{
		char *prompt;
		prompt = readline("> ");
		parse_prompt(prompt, env);
	}
	return (0);
}
