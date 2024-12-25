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
	char *middle;
	int start_quotes;
	int end_quotes;
	int len;
	int i;
	int j;

	start_quotes = count_start_quotes(word, word[0]);
	end_quotes = count_end_quotes(word, word[0]);
	len = ft_strlen(word);
	if (len <= 2)
	{
		ft_printf("word between quotes is too short\n");
		return (ft_strdup(""));
	}
	middle = malloc(sizeof(char) * ((len - quote_count) + 1));
	if (!middle)
		return (NULL);
	i = start_quotes;
	j = 0;
	while (i < len - end_quotes)
	{
		middle[j] = word[i];
		i++;
		j++;
	}
	middle[j] = '\0';
	return (middle);
}


int parse_prompt(char *prompt, char **env)
{
	char **res;
	int i = -1;
	int total_quotes;

	if (!ft_strncmp(prompt, "exit", 4))
		exit (EXIT_FAILURE);
	res = ft_split(prompt, ' ');
	while (res[++i])
	{
		ft_printf("%s\n", res[i]);
		total_quotes = count_start_quotes(res[i], res[i][0]) + count_end_quotes(res[i], res[i][0]);
		if (build_path(res[i], env))
			ft_printf("Is a command\n");
		else if (total_quotes % 2 == 0) //count_quotes(res[i], res[i][0]) != 1
		{
			printf("Amount of quotes : %d\n", total_quotes);
			if (build_path(get_command(res[i], count_quotes(res[i], res[i][0])), env))
				printf("Is between even quotes, and is a command\n");
		}
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
