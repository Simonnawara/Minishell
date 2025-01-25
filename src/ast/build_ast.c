/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/25 15:39:08 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_argument_to_command(t_ast_node *cmd_node, char *arg)
{
	int		i;
	int		j;
	char	**new_args;

	i = 0;
	j = -1;
	if (cmd_node->args) // Count existing arguments
		while (cmd_node->args[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (0);
	while (++j < i) // Copy existing arguments
	{
		new_args[j] = ft_strdup(cmd_node->args[j]);
		if (!new_args[j])
		{
			while (--j >= 0)
				free(new_args[j]);
            free(new_args);
            return (0);
		}
	}
	//new_args[i] = ft_strdup(arg); // Add new argument
	if (!(new_args[i] = ft_strdup(arg)))
    {
        while (--j >= 0)
            free(new_args[j]);
        free(new_args);
        return (0);
    }
	new_args[i + 1] = NULL;
	if (cmd_node->args) // Free old arguments array
		free_array(cmd_node->args);
	cmd_node->args = new_args;
	return (1);
}

t_ast_node	*create_ast_node(t_token_type type, char *value)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	node->left = NULL;
	node->right = NULL;
	node->args = NULL;
	return (node);
}

t_ast_node	*handle_redirection_sequence(t_token **tokens)
{
	t_token		*current;
	t_ast_node	*cmd_node;
	t_ast_node	*root;
	t_ast_node	*prev_redir;
	t_ast_node	*redir;

	current = *tokens;
	cmd_node = NULL;
	root = NULL;
	prev_redir = NULL;
	// First build command node if it exists
	while (current && current->type != T_COMMAND)
		current = current->next;
	if (current)
	{
		cmd_node = build_command_node(&current);
		if (!cmd_node)
			return (NULL);
	}
	// Reset to beginning for redirections
	current = *tokens;
	while (current)
	{
		if ((current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT
				|| current->type == T_APPEND || current->type == T_HEREDOC)
			&& current->next)
		{
			redir = create_ast_node(current->type, current->value);
			if (!redir)
			{
				return (NULL);
				free_ast(root);
				free_ast_node(cmd_node);
			}
			if (!current->next || current->next->type != T_WORD)
			{
				free_ast_node(redir);
				free_ast(root);
				free_ast_node(cmd_node);
				return (NULL);
			}
			redir->right = create_ast_node(T_WORD, current->next->value);
			if (!redir->right)
			{
				free_ast_node(redir);
				free_ast(root);
				free_ast_node(cmd_node);
				return (NULL);
			}
			if (!root)
			{
				root = redir;
				if (cmd_node)
					root->left = cmd_node;
			}
			else if (prev_redir)
			{
				prev_redir->left = redir;
				if (cmd_node && !redir->left)
					redir->left = cmd_node;
			}
			prev_redir = redir;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	return (root ? root : cmd_node);
}

t_ast_node	*build_ast(t_token **tokens)
{
	t_token		*current;
	t_ast_node	*pipe;
	t_token		*left;
	t_token		*right;
	t_token		*split;

	if (!tokens || !*tokens)
		return (NULL);
	// Handle pipes first
	current = *tokens;
	while (current)
	{
		if (current->type == T_PIPE)
		{
			pipe = create_ast_node(T_PIPE, "|");
			if (!pipe)
				return (NULL);
			left = *tokens;
			right = current->next;
			// Split tokens
			split = *tokens;
			while (split && split->next != current)
				split = split->next;
			if (split)
				split->next = NULL;
			pipe->left = build_ast(&left);
			pipe->right = build_ast(&right);
			if (!pipe->left || !pipe->right)
			{
				free_ast_node(pipe);
				return (NULL);
			}
			*tokens = NULL;
			return (pipe);
		}
		current = current->next;
	}
	return (handle_redirection_sequence(tokens));
}

t_ast_node	*build_command_node(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_token		*current;

	current = *tokens;
	if (!current)
	{
		// printf("No tokens to build command node\n");
		return (NULL);
	}
	// printf("Building command node for: %s\n", current->value);
	cmd_node = create_ast_node(T_COMMAND, current->value);
	if (!cmd_node)
		return (NULL);
	cmd_node->res = current->res;
	cmd_node->echo_counter = current->echo_counter;
	// printf("Adding command as first argument\n");
	if (!add_argument_to_command(cmd_node, current->value))
	{
		// printf("Failed to add command as argument\n");
		free_ast_node(cmd_node);
		return (NULL);
	}
	current = current->next; // Move to the next token after command
	while (current && current->type == T_WORD)
	{
		// printf("Adding argument: %s\n", current->value);
		if (!add_argument_to_command(cmd_node, current->value))
		{
			// printf("Failed to add argument\n");
			free_ast_node(cmd_node);
			return (NULL);
		}
		current = current->next;
	}
	*tokens = current; // Update token pointer
	return (cmd_node);
}
