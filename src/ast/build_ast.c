/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/15 12:13:02 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int add_argument_to_command(t_ast_node *cmd_node, char *arg)
{
    int i;
	int j;
    char **new_args;

	i = 0;
	j = -1;
    if (cmd_node->args)  // Count existing arguments
    {
        while (cmd_node->args[i])
            i++;
    }
    new_args = malloc(sizeof(char *) * (i + 2)); // Allocate new array with space for one more argument
    if (!new_args)
		return 0;
	while (++j < i) // Copy existing arguments
		new_args[j] = ft_strdup(cmd_node->args[j]);
    new_args[i] = ft_strdup(arg); // Add new argument
    new_args[i + 1] = NULL;
    if (cmd_node->args) // Free old arguments array
		free_array(cmd_node->args);
    cmd_node->args = new_args;
    return 1;
}

t_ast_node *create_ast_node(t_token_type type, char *value)
{
	t_ast_node *node;

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

t_ast_node *build_command_node(t_token **tokens)
{
	t_ast_node *cmd_node;
	t_token *current;

	current = *tokens;
	if (!current)
		return (NULL);
	cmd_node = create_ast_node(T_COMMAND, current->value);
	if (!cmd_node)
		return (NULL);
	if (!add_argument_to_command(cmd_node, current->value))  // Add command itself as first argument
    {
        free_ast_node(cmd_node);
        return NULL;
    }
	current = current->next; // Move to the next token after command
	while(current && current->type == T_WORD)
	{
		if (!add_argument_to_command(cmd_node, current->value))
			return (free_ast_node(cmd_node), NULL);
		current = current->next;
	}
	*tokens = current;
	//printf("cmd_node : %s\n", cmd_node->value);
	return (cmd_node);
}

t_ast_node	*build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_token		*current;

	current = *tokens;
	if (!current)
		return (NULL);
	if (current->type == T_COMMAND)
		return (build_command_node(tokens));
	if (current->type == T_PIPE || current->type == T_AND
		|| current->type == T_OR)
	{
		printf("laAAAAAAAAAAAAA\n");
		printf("current->type : %d\n", current->type);
		root = create_ast_node(current->type, current->value);
		if (!root)
			return (NULL);
		*tokens = current->next;
		root->left = build_ast(tokens);
		if (!root->left)
			return (free_ast_node(root), NULL);
		root->right = build_ast(tokens);
		if (!root->right)
			return (free_ast_node(root), NULL);
		return (root);
	}
	if (current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT ||
    current->type == T_APPEND)
	{
		root = create_ast_node(current->type, current->value);
		if (!root)
			return (NULL);
		*tokens = current->next;
		if (!*tokens || (*tokens)->type != T_WORD) // Redirection needs a file name
			return (free_ast_node(root), NULL);
		root->right = create_ast_node(T_WORD, (*tokens)->value);
		if (!root->right)
			return (free_ast_node(root), NULL);
		*tokens = (*tokens)->next;
		root->left = build_ast(tokens);
		return (root);
	}
	return (NULL);
}
