/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:51:03 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/23 11:55:52 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include "../libft/include/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_path
{
	char		**paths;
	char		*path_var;
	char		*full_path;
	char		*temp;
}				t_path;

typedef enum e_token_type
{
    T_WORD,         // Regular words and quoted strings
    T_COMMAND,      // Known shell commands
    T_REDIRECT_IN,  // <
    T_REDIRECT_OUT, // >
    T_APPEND,       // >>
    T_PIPE,         // |
    T_AND,          // &&
    T_OR,           // ||
    T_SEMICOLON,    // ;
    T_PAREN_L,      // (
    T_PAREN_R       // )
} t_token_type;

typedef struct s_token
{
    char        *value;
    t_token_type type;
    struct s_token *next;
} t_token;

/* typedef struct s_command
{
    char    *name;
    char    **args;
    char    *input_file;
    char    *output_file;
    int     append_output;
    struct s_command *next;  // For piped commands
} t_command; */

typedef struct s_command_table
{
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		pipe_out;
}	t_command_table;

typedef struct s_ast_node
{
	t_token_type type;
	char *value;
	char **args;
	struct s_ast_node *left;
	struct s_ast_node *right;
}	t_ast_node;


// main.c //
char *get_command(char *word, int quote_count, char quote_type);

// free.c //
void	free_array(char **array);
void	*free_and_return(char **array, void *return_value);
char	**free_split(char **res);
void	free_token_list(t_token *tokens);
void	free_ast_node(t_ast_node *node);
void	free_ast(t_ast_node *root);
void	free_command_table(t_command_table *cmd);

// path.c //
char	*get_path(char **env);
char	*build_path(char *cmd, char **env);

// quotes.c //
int count_quotes(char *word, char quote);
int get_quote_type(char start_quote, char end_quote);

// token_builder.c //
char **tokenize(char *prompt);
int count_words(const char *str);

// token_list.c //
t_token_type	get_operator_type(char *token);
t_token_type	classify_token(char *token, char **env);

// build_ast_utils.c //
t_command_table	*init_command_table(void);
int	add_argument_to_command(t_ast_node *cmd_node, char *arg);
int	execute_command(t_ast_node *cmd_node, char **env);
int	execute_pipe(t_ast_node *left, t_ast_node *right, char **env);
t_command_table	*convert_node_to_command(t_ast_node *node);
char	**copy_string_array(char **arr);

// build_ast.c //
t_ast_node *create_ast_node(t_token_type type, char *value);
t_ast_node *build_command_node(t_token **tokens);
t_ast_node	*build_ast(t_token **tokens);
void print_ast(t_ast_node *root);
void print_full_ast(t_ast_node *root);


// execute_ast.c //
void	execute_ast(t_ast_node *root, char **env);

// tokenize_2.0.c //
//t_token	*tokenize_input(char *input);
t_token	*tokenize_input(char *input, char **env);
int	get_word_length(char *str);
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **list, t_token *new);
char *extract_quoted_token(char *str, int *i, char quote);
void print_token_info(t_token *token);

#endif