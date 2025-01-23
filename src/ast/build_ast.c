/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:59:54 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/23 13:28:08 by trouilla         ###   ########.fr       */
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
// static t_ast_node *handle_heredoc_command(t_token **tokens, t_token *heredoc_token)
// {
//     t_ast_node  *root;
//     t_token     *command_tokens = NULL;
//     t_token     *next_tokens;
//     t_ast_node  *command_node = NULL;
//     t_token     *current = *tokens;

//     // Set up heredoc node
//     root = create_ast_node(T_HEREDOC, heredoc_token->value);
//     if (!root)
//         return (NULL);

//     // Set delimiter
//     root->right = create_ast_node(T_WORD, heredoc_token->next->value);
//     if (!root->right)
//     {
//         free_ast_node(root);
//         return (NULL);
//     }

//     // Save tokens after heredoc
//     next_tokens = heredoc_token->next->next;

//     // Find redirection tokens before heredoc
//     while (current && current != heredoc_token)
//     {
//         if (current->type == T_REDIRECT_IN)
//         {
//             t_ast_node *redir = create_ast_node(T_REDIRECT_IN, current->value);
//             if (!redir)
//             {
//                 free_ast_node(root);
//                 return (NULL);
//             }
//             redir->right = create_ast_node(T_WORD, current->next->value);
//             redir->left = root;
//             root = redir;
//             break;
//         }
//         current = current->next;
//     }

//     // Handle command node
//     if (*tokens != heredoc_token)
//     {
//         command_tokens = *tokens;
//         command_node = build_command_node(&command_tokens);
//         if (command_node)
//         {
//             // Find leftmost node to attach command
//             t_ast_node *leftmost = root;
//             while (leftmost->left)
//                 leftmost = leftmost->left;
//             leftmost->left = command_node;
//         }
//     }

//     *tokens = next_tokens;
//     return root;
// }

t_ast_node *build_ast(t_token **tokens)
{
    t_ast_node *root = NULL;
    t_token *current = *tokens;
    t_token *heredoc_token = NULL;
    t_token *redir_token = NULL;

    if (!tokens || !*tokens)
        return NULL;

    // First find command token
    while (current && current->type != T_COMMAND)
        current = current->next;
    
    if (!current)
        return NULL;

    // Create initial command node
    root = create_ast_node(T_COMMAND, current->value);
    if (!root || !add_argument_to_command(root, current->value))
        return NULL;

    // First handle file redirection
    current = *tokens;
    while (current)
    {
        if (current->type == T_REDIRECT_IN)
        {
            redir_token = current;
            t_ast_node *redir = create_ast_node(T_REDIRECT_IN, redir_token->value);
            if (!redir)
                return NULL;
            redir->right = create_ast_node(T_WORD, redir_token->next->value);
            if (!redir->right)
            {
                free_ast_node(redir);
                return NULL;
            }
            redir->left = root;
            root = redir;
        }
        current = current->next;
    }

    // Then handle heredoc
    current = *tokens;
    while (current)
    {
        if (current->type == T_HEREDOC)
        {
            heredoc_token = current;
            t_ast_node *heredoc = create_ast_node(T_HEREDOC, heredoc_token->value);
            if (!heredoc)
                return NULL;
            heredoc->right = create_ast_node(T_WORD, heredoc_token->next->value);
            if (!heredoc->right)
            {
                free_ast_node(heredoc);
                return NULL;
            }
            heredoc->left = root;
            root = heredoc;
        }
        current = current->next;
    }

    *tokens = NULL;
    return root;
}
// t_ast_node *build_ast(t_token **tokens)
// {
//     t_ast_node  *root;
//     t_token     *current;
//     t_token     *pipe_token = NULL;
//     t_token     *heredoc_token = NULL;
//     t_token     *redir_token = NULL;
//     t_token     *split_point = NULL;
//     t_token     *left_tokens = NULL;
//     t_token     *right_tokens = NULL;

// 	current = *tokens;
//     if (!current)
//         return (NULL);

//     // First, look for heredoc as it should be handled first
//     while (current)
//     {
//         if (current->type == T_HEREDOC)
//         {
//             heredoc_token = current;
//             break;
//         }
//         current = current->next;
//     }

//     if (heredoc_token)
//         return (handle_heredoc_command(tokens, heredoc_token));

//     // Then look for pipe
//     current = *tokens;
//     while (current)
//     {
//         if (current->type == T_PIPE)
//         {
//             pipe_token = current;
//             break;
//         }
//         current = current->next;
//     }

//     // Handle pipe
//     if (pipe_token)
//     {
//         left_tokens = *tokens;
//         right_tokens = pipe_token->next;
//         split_point = *tokens;
//         while (split_point && split_point->next != pipe_token)
//             split_point = split_point->next;
//         if (split_point)
//             split_point->next = NULL;
//         root = create_ast_node(T_PIPE, "|");
//         if (!root)
//             return (NULL);
//         root->left = build_ast(&left_tokens);
//         if (!root->left)
//         {
//             free_ast_node(root);
//             return (NULL);
//         }
//         root->right = build_ast(&right_tokens);
//         if (!root->right)
//         {
//             free_ast_node(root);
//             return (NULL);
//         }
//         *tokens = NULL;
//         return (root);
//     }
//     current = *tokens;
//     while (current)
//     {
//         if (current->type == T_REDIRECT_IN ||
//             current->type == T_REDIRECT_OUT ||
//             current->type == T_APPEND)
//         {
//             redir_token = current;
//             break;
//         }
//         current = current->next;
//     }
//     if (redir_token)
//     {
//         if (!redir_token->next || redir_token->next->type != T_WORD)
//             return (NULL);
//         root = create_ast_node(redir_token->type, redir_token->value);
//         if (!root)
//             return (NULL);
//         root->right = create_ast_node(T_WORD, redir_token->next->value);
//         if (!root->right)
//         {
//             free_ast_node(root);
//             return (NULL);
//         }
//         left_tokens = *tokens;
//         split_point = *tokens;
//         while (split_point && split_point->next != redir_token)
//             split_point = split_point->next;
//         if (split_point)
//             split_point->next = NULL;
//         *tokens = redir_token->next->next;
//         root->left = build_ast(&left_tokens);
//         if (!root->left)
//         {
//             free_ast_node(root);
//             return (NULL);
//         }
//         return root;
//     }
//     return build_command_node(tokens);
// }

t_ast_node *build_command_node(t_token **tokens)
{
    t_ast_node *cmd_node;
    t_token *current;

    current = *tokens;
    if (!current)
    {
        //printf("No tokens to build command node\n");
        return (NULL);
    }

    //printf("Building command node for: %s\n", current->value);
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
        return NULL;
    }

    current = current->next; // Move to the next token after command
    while(current && current->type == T_WORD)
    {
        // printf("Adding argument: %s\n", current->value);
        if (!add_argument_to_command(cmd_node, current->value))
        {
            // printf("Failed to add argument\n");
            free_ast_node(cmd_node);
            return NULL;
        }
        current = current->next;
    }

    *tokens = current;  // Update token pointer
    // printf("Command node built successfully with value: %s\n", cmd_node->value);
    return cmd_node;
}
