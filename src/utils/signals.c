/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:16:44 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 16:17:22 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;
    g_exit_status = 130;
    write(STDERR_FILENO, "\n", 1);
    rl_on_new_line();      // Move to a new line
    rl_replace_line("", 0); // Clear the current input line
    rl_redisplay();        // Redisplay the prompt
}

void handle_heredoc_sigint(int sig)
{
    (void)sig;
    g_exit_status = 130;
    write(STDERR_FILENO, "\n", 1);
	close(0);  // Close stdin to force readline to return
}


void	setup_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

void setup_heredoc_signals(void)
{
    signal(SIGINT, handle_heredoc_sigint);
    signal(SIGQUIT, SIG_IGN);
}


void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void ignore_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
