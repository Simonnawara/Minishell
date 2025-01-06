/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:23:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/06 16:32:35 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	file_error(char *filename)
{
	write(2, filename, ft_strlen(filename));
	perror(" ");
}

int	is_command_found(char *word, char **env)
{
	t_token_type	type;
	type = classify_token(word, env);
	if (type == T_WORD)
	{
		write(2, word, ft_strlen(word));
		ft_putendl_fd(" : command not found", 2);
		return (1);
	}
	return (0);
}
