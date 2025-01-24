/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:14:38 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/24 13:24:48 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int g_exit_status = 0;


void update_exit_status(int status)
{
    // Update the global exit status
    // For signals, status is typically signal number + 128
    if (WIFSIGNALED(status))
        g_exit_status = 128 + WTERMSIG(status);
    else if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else
        g_exit_status = status;
}

char *get_exit_status(void)
{
    char *exit_str;
    
    // Convert the exit status to a string
    exit_str = ft_itoa(g_exit_status);
    if (!exit_str)
        return (ft_strdup("0")); // Default to "0" if allocation fails
        
    return exit_str;
}
