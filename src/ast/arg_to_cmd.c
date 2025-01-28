/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_to_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:39:44 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 14:52:06 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_new_args(char **new_args, int count)
{
	while (--count >= 0)
		free(new_args[count]);
	free(new_args);
}

static int	copy_existing_args(char **new_args, char **old_args, int count)
{
	int	j;

	j = -1;
	while (++j < count)
	{
		new_args[j] = ft_strdup(old_args[j]);
		if (!new_args[j])
		{
			free_new_args(new_args, j);
			return (0);
		}
	}
	return (1);
}

static char	**allocate_new_args(char **old_args, int *count)
{
	char	**new_args;

	*count = 0;
	if (old_args)
		while (old_args[*count])
			(*count)++;
	new_args = malloc(sizeof(char *) * (*count + 2));
	if (!new_args)
		return (NULL);
	return (new_args);
}

int	add_argument_to_command(t_ast_node *cmd_node, char *arg)
{
	int		count;
	char	**new_args;

	new_args = allocate_new_args(cmd_node->args, &count);
	if (!new_args)
		return (0);
	if (!copy_existing_args(new_args, cmd_node->args, count))
		return (0);
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
	{
		free_new_args(new_args, count);
		return (0);
	}
	new_args[count + 1] = NULL;
	if (cmd_node->args)
		free_array(cmd_node->args);
	cmd_node->args = new_args;
	return (1);
}

