/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:27:29 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/24 12:49:04 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command_table	*init_command_table(void)
{
	t_command_table	*cmd;

	cmd = malloc(sizeof(t_command_table));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->pipe_out = 0;
	return (cmd);
}

/* Conversion Functions */

t_command_table	*convert_node_to_command(t_ast_node *node)
{
	t_command_table	*cmd;

	if (!node || node->type != T_COMMAND)
		return (NULL);
	cmd = init_command_table();
	if (!cmd)
		return (NULL);
	cmd->cmd = ft_strdup(node->value);
	cmd->args = copy_string_array(node->args);
	if (!cmd->cmd || !cmd->args)
	{
		free_command_table(cmd);
		return (NULL);
	}
	return (cmd);
}

char	**copy_string_array(char **arr)
{
	char	**new;
	int		i;
	int		len;

	if (!arr)
		return (NULL);
	len = 0;
	while (arr[len])
		len++;
	new = malloc(sizeof(char *) * (len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = ft_strdup(arr[i]);
		if (!new[i])
		{
			free_array(new);
			return (NULL);
		}
		i++;
	}
	new[i] = NULL;
	return (new);
}
