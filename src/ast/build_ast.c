/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/03 14:00:29 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return (node);
}

t_ast_node *build_command_node(t_token **tokens)
{
	t_ast_node *cmd_node;
	t_token *current;

	current = *tokens;
	cmd_node = create_ast_node(T_COMMAND, current->value);
	if (!cmd_node)
		return (NULL);
	current = current->next;
	while(current && current->type == T_WORD)
	{
		if (!add_argument_to_command(cmd_node, current->value))
			return (free_ast_node(cmd_node), NULL);
		current = current->next;
	}
	*tokens = current;
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
	// Add to build_ast function
	if (current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT || 
    current->type == T_APPEND)
	{
		root = create_ast_node(current->type, current->value);
		if (!root)
			return (NULL);
		*tokens = current->next;
		// Redirection needs a file name
		if (!*tokens || (*tokens)->type != T_WORD)
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

void print_ast(t_ast_node *root)
{
    if (!root)
        return;

    // Print the type of the current node
    if (root->type == T_COMMAND)
        printf("Command: %s\n", root->value);
    else if (root->type == T_WORD)
        printf("Word: %s\n", root->value);
    else if (root->type == T_PIPE)
        printf("Pipe\n");
    else if (root->type == T_AND)
        printf("AND\n");
    else if (root->type == T_OR)
        printf("OR\n");
    else if (root->type == T_REDIRECT_IN)
        printf("Redirect In: %s\n", root->value);
    else if (root->type == T_REDIRECT_OUT)
        printf("Redirect Out: %s\n", root->value);
    else if (root->type == T_APPEND)
        printf("Append Redirect: %s\n", root->value);
    else if (root->type == T_SEMICOLON)
        printf("Semicolon\n");

    // Recursively print the left and right subtrees
    if (root->left)
    {
        printf("Left Child:\n");
        print_ast(root->left);
    }

    if (root->right)
    {
        printf("Right Child:\n");
        print_ast(root->right);
    }
}

// Helper function to start the AST printing process
void print_full_ast(t_ast_node *root)
{
    if (!root)
    {
        printf("AST is empty!\n");
        return;
    }

    printf("AST:\n");
    print_ast(root);
}