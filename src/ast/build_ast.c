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

/* t_ast_node *build_command_node(t_token **tokens)
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
} */

/* t_ast_node	*build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_token		*current;

	current = *tokens;
	printf("current->type : %d\n", current->type);
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
} */

t_ast_node *build_ast(t_token **tokens)
{
    t_ast_node  *root;
    t_token     *current;
    t_token     *pipe_token = NULL;
    t_token     *split_point = NULL;
    t_token     *left_tokens = NULL;
    t_token     *right_tokens = NULL;

    current = *tokens;
    printf("\n=== Starting build_ast ===\n");
    //printf("First token: %s (type: %d)\n", current ? current->value : "NULL", current ? current->type : -1);
    
    if (!current)
        return (NULL);

    // First, look for a pipe operator at the top level
    current = *tokens;
    while (current)
    {
        printf("Scanning token: %s (type: %d)\n", current->value, current->type);
        if (current->type == T_PIPE)
        {
            pipe_token = current;
            printf("Found pipe token at: %s\n", current->value);
            break;
        }
        current = current->next;
    }

    // If we found a pipe, split into left and right subtrees
    if (pipe_token)
    {
        printf("Processing pipe operator\n");
        
        // Create new token lists for left and right sides
        left_tokens = *tokens;
        right_tokens = pipe_token->next;
        
        // Find the token before pipe to terminate left list
        split_point = *tokens;
        while (split_point && split_point->next != pipe_token)
            split_point = split_point->next;
        
        if (split_point)
        {
            printf("Splitting tokens at: %s\n", split_point->value);
            split_point->next = NULL;  // Terminate left group
        }

        printf("Left tokens start with: %s\n", left_tokens ? left_tokens->value : "NULL");
        printf("Right tokens start with: %s\n", right_tokens ? right_tokens->value : "NULL");

        // Create pipe node
        root = create_ast_node(T_PIPE, "|");
        if (!root)
            return (NULL);
        
        printf("Building left subtree...\n");
        root->left = build_ast(&left_tokens);
        if (!root->left)
        {
            printf("Failed to build left subtree\n");
            free_ast_node(root);
            return (NULL);
        }

        printf("Building right subtree...\n");
        root->right = build_ast(&right_tokens);
        if (!root->right)
        {
            printf("Failed to build right subtree\n");
            free_ast_node(root);
            return (NULL);
        }

        printf("Successfully built pipe node with both subtrees\n");
        *tokens = NULL;  // Consume all tokens
        return root;
    }
    
    // If no pipe, build a command node
    printf("No pipe found, building command node\n");
    root = build_command_node(tokens);
    printf("Built command node: %s\n", root ? root->value : "NULL");
    return root;
}

t_ast_node *build_command_node(t_token **tokens)
{
    t_ast_node *cmd_node;
    t_token *current;

    printf("\n=== Starting build_command_node ===\n");
	printf("Test pour git push\n");
    current = *tokens;
    if (!current)
    {
        printf("No tokens to build command node\n");
        return (NULL);
    }

    printf("Building command node for: %s\n", current->value);
    cmd_node = create_ast_node(T_COMMAND, current->value);
    if (!cmd_node)
        return (NULL);

    printf("Adding command as first argument\n");
    if (!add_argument_to_command(cmd_node, current->value))
    {
        printf("Failed to add command as argument\n");
        free_ast_node(cmd_node);
        return NULL;
    }

    current = current->next; // Move to the next token after command
    while(current && current->type == T_WORD)
    {
        printf("Adding argument: %s\n", current->value);
        if (!add_argument_to_command(cmd_node, current->value))
        {
            printf("Failed to add argument\n");
            free_ast_node(cmd_node);
            return NULL;
        }
        current = current->next;
    }

    *tokens = current;  // Update token pointer
    printf("Command node built successfully with value: %s\n", cmd_node->value);
    return cmd_node;
}
