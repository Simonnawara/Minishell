/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:51:45 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/18 10:56:14 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../includes/minishell.h"

static	int		nb_args(char **args)
{
	int		size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

/* int				ft_echo(char **args, char **res, int echo_counter)
{
	int		i;
	int		n_option;
	//t_token_type type;

	if (!args)
    	return (1);
	i = 1;
	n_option = 0;
	//printf("echo counter in echo : %d\n" , echo_counter);
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			//type = classify_token(args[i], env);
			ft_putstr_fd(args[i], 1);
			if (args[i + 1] && args[i][0] != '\0')
				write(1, " ", 1);
			i++;
			//printf(" res[echo_counter + 1] in echo : %s\n", res[echo_counter + 1]);
			echo_counter++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
} */


int				ft_echo(char **args, char **res, int echo_counter)
{
	int		i;
	int		n_option;
	int		total_quotes;
	char	quote_type;
	char	*cmd;

	if (!args)
    	return (1);
	i = 1;
	n_option = 0;
	if (nb_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (res[echo_counter + i])
		{
			quote_type = 0;
			if (res[echo_counter + i][0] == res[echo_counter + i][ft_strlen(res[echo_counter + i]) - 1]
				&& (res[echo_counter + i][0] == 34 || res[echo_counter + i][0] == 39))
			{
				quote_type = res[echo_counter + i][0];
				printf("quote_type : %c\n", quote_type);
				total_quotes = count_quotes(res[echo_counter + i], quote_type);
				cmd = get_command(res[echo_counter + i], total_quotes, quote_type);
				ft_putstr_fd(cmd, 1);
				free(cmd);
			}
			else
				ft_putstr_fd(args[i], 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
