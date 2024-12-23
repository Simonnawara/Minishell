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

int validate_inputs(int argc, char **argv)
{
	if (argc < 1)
		return (ft_putendl_fd("Error : argument number invalid", 2), EXIT_FAILURE);
	(void)argv;
	return (0);
}

int main(int argc, char **argv)
{
	validate_inputs(argc, argv);
	while (1)
	{
		char *prompt;
		prompt = readline("> ");
		printf("%s\n", prompt);
	}
	return (0);
}