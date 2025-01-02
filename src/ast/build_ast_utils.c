/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:27:29 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/02 16:53:01 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Command Building Functions */

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

int	add_argument_to_command(t_ast_node *cmd_node, char *arg)
{
	char	**new_args;
	int		arg_count;
	int		i;

	arg_count = 0;
	while (cmd_node->args && cmd_node->args[arg_count])
		arg_count++;
	new_args = malloc(sizeof(char *) * (arg_count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < arg_count)
	{
		new_args[i] = cmd_node->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd_node->args);
	cmd_node->args = new_args;
	return (1);
}

/* Execution Functions */

int	execute_command(t_ast_node *cmd_node, char **env)
{
	t_command_table	*cmd;
	pid_t			pid;
	int				status;
	char			*path;

	cmd = convert_node_to_command(cmd_node);
	if (!cmd)
		return (1);
	pid = fork();
	if (pid == 0)
	{
		path = build_path(cmd->cmd, env);
		if (!path)
			exit(127);
		if (execve(path, cmd->args, env) == -1)
			exit(126);
	}
	waitpid(pid, &status, 0);
	free_command_table(cmd);
	return (WEXITSTATUS(status));
}

int	execute_pipe(t_ast_node *left, t_ast_node *right, char **env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_command(left, env));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_command(right, env));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
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

/* Utility Functions */

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