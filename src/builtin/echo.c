/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:37:07 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/11 09:51:40 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_n_flag(char *arg)
{
    int i;

    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return (0);
    i = 2;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

static int has_n_option(char **args, int *i)
{
    int n_flag;

    n_flag = 0;
    while (args[*i] && is_n_flag(args[*i]))
    {
        n_flag = 1;
        (*i)++;
    }
    return (n_flag);
}

static void print_args(char **args, int i)
{
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
}

int ft_echo(char **args)
{
    int i;
    int n_flag;

    i = 1;
    if (!args[i])
    {
        ft_putchar_fd('\n', 1);
        return (0);
    }
    n_flag = has_n_option(args, &i);
    print_args(args, i);
    if (!n_flag)
        ft_putchar_fd('\n', 1);
    return (0);
}
