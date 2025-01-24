/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 11:16:44 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/24 11:25:06 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/globals.h"

extern int g_exit_status;

static void	handle_sigint(int sig)
{
    (void)sig;
    g_exit_status = 130;
    write(STDERR_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	setup_signals(void)
{
    signal(SIGINT, handle_sigint);   // Handle Ctrl-C
    signal(SIGQUIT, SIG_IGN);        // Ignore Ctrl-
}

void	reset_signals(void)
{
    // Reset signals to default behavior
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void	ignore_signals(void)
{
    // Ignore signals (useful for child processes)
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
