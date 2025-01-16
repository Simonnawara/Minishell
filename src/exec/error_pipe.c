/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:33:14 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/16 10:03:16 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// //fonction pour close pipe proprement
// void close_pipe(int pipe_fd[2])
// {
// 	if (pipe_fd[0] != -1)
// 	{
// 		close(pipe_fd[0]);
// 		pipe_fd[0] = -1;
// 	}
// 	if (pipe_fd[1] != -1)
// 	{
// 		close(pipe_fd[1]);
// 		pipe_fd[1] = -1;
// 	}
// }
// //Gere les erreurs pipe + message approprie
// //return code erreur pipe
// int pipe_error(void)
// {
// 	char *msg;

// 	msg = strerror(errno);
// 	ft_putstr_fd("minishell :pipe error: ", 2);
// 	ft_putendl_fd(msg, 2);
// 	return (1);
// }
// //gere les erreur fork + message approprie
// //A checker si bon code erreur
int fork_error(void)
{
	char *msg;

	msg = strerror(errno);
	ft_putstr_fd("minishell: fork error: ", 2);
	ft_putendl_fd(msg, 2);
	return (errno == EAGAIN ? 11 : 1);
}
