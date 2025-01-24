/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:59:24 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/21 10:59:24 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include "../libft/include/libft.h"
# include <fcntl.h>
# include <linux/limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>

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
	T_HEREDOC,		// <<
    T_SEMICOLON,    // ;
    T_PAREN_L,      // (
    T_PAREN_R,       // )
	T_BUILTIN
} t_token_type;

typedef struct s_token
{
    char        *value;
	char		**res;
	int			echo_counter;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef struct s_command_table
{
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
	char *heredoc_file;
    char *delimiter;
	char		**res;
	int			echo_counter;
    int heredoc;
    int append;
    int saved_stdin;
    int saved_stdout;
	int		pipe_out;
	char    **all_outfiles; // Array of all output files in order
    int     num_outfiles;   // Number of output files
}	t_command_table;



typedef struct s_ast_node
{
	t_token_type type;
	char *value;
	char **args;
	char		**res;
	int			echo_counter;
	struct s_ast_node *left;
	struct s_ast_node *right;
}	t_ast_node;

typedef struct s_exec
{
	t_ast_node *ast; //avoir argument commande + type
	t_path *path_info; //info sur chemin cmd
	int last_status; // Dernier code de retour
	int pipe_read; // Descripteur lecture pipe
	int pipe_write; // Descripteur ecriture pipe
	char **env; //Environnement
	int *env_allocated;//checker var alloue moi meme
	int compteur_pipe;
}	t_exec;

typedef struct s_heredoc
{
	char *filename;
	int fd;
} t_heredoc;


// main.c //
char *get_command(char *word, int quote_count, char quote_type);
char **init_env(char **original_env);

// free.c //
void	free_array(char **array);
void	*free_and_return(char **array, void *return_value);
int		free_word_and_return(char *word, int return_value);
int		free_array_and_return(char **array, int return_value);
void	free_token_list(t_token *tokens);
void	free_ast_node(t_ast_node *node);
void	free_ast(t_ast_node *root);
void	free_command_table(t_command_table *cmd);

// errors.c //
void	file_error(char *filename);
int	is_command_found(char *word, char **env);
int verify_forbidden_tokens(char *prompt);
int ft_strcmp(const char *s1, const char *s2);
void	*ft_memcpy(void *dest, const void *src, size_t n);

//error_pipe.c
int pipe_error(void);
void close_pipe(int pipe_fd[2]);
int fork_error(void);

// path.c //
char	*get_path(char **env);
char	*build_path(char *cmd, char **env);

// quotes.c //
int count_quotes(char *word, char quote);
int get_quote_type(char start_quote, char end_quote);

// tokenize.c //
char **tokenize(char *prompt);
int count_words(const char *str);
int move_past_quotes(const char *str, char quote_type, int *i);

// token_type.c //
t_token_type	get_operator_type(char *token);
t_token_type	classify_token(char *token, char **env);
t_token_type	classify_token_prev(char *token, char **env, t_token_type prev_type);
int check_pipe(t_token_type type, char **res, int i);
int check_redirect(t_token_type type, char **res, int i);

// build_ast_utils.c //
t_command_table	*init_command_table(void);
int	add_argument_to_command(t_ast_node *cmd_node, char *arg);
//int	execute_command(t_ast_node *cmd_node, char **env);
//int	execute_pipe(t_ast_node *left, t_ast_node *right, char **env);
t_command_table	*convert_node_to_command(t_ast_node *node);
char	**copy_string_array(char **arr);

// build_ast.c //
t_ast_node *create_ast_node(t_token_type type, char *value);
t_ast_node *build_command_node(t_token **tokens);
t_ast_node	*build_ast(t_token **tokens);
void print_ast(t_ast_node *root);
void print_full_ast(t_ast_node *root);
t_ast_node *handle_redirections(t_token **tokens);
t_ast_node *handle_multiple_redirections(t_token **tokens);

// execute_ast.c //
int execute_ast(t_ast_node *ast, t_exec *exec);

//execute_pipe.c
int execute_pipe_node(t_ast_node *node, t_exec *exec);
//void	free_node(t_ast_node *node);

//execute_logical_operator
int execute_logical_node(t_ast_node *node, t_exec *exec);

//exec_simple_command.c
int execute_simple_command(t_ast_node *node, t_exec *exec, t_command_table cmd);
int is_builtin(char *cmd);

//Builtin *.c
//int				ft_echo(char **args, char **res, int echo_counter);
int	ft_echo(char **args, char **res, int echo_counter, t_exec *exec);
char *expand_variables(char *str, char **env);
int	ft_pwd();
int	ft_export(char **args, t_exec *exec);
int	ft_unset(t_exec *exec, char **args);
int	ft_env(t_exec *exec, char **args);
int	ft_exit(t_ast_node *node, t_exec *exec);
int ft_cd(char **args, t_exec *exec);

//redirection_input
int setup_redirection(t_command_table *cmd);
int restore_io(t_command_table *cmd);
//set_env_cd.c
int	update_env_variable(char **env, char *key, char *value);
//external command .c
int	execute_external_command(t_command_table *cmd, t_exec *exec);
//extern_cmd_path
char	*find_command_path(char *cmd, t_path *path_info);
char	*search_in_path(t_path *path_info, char *cmd);

//Heredoc.c
int execute_heredoc(t_ast_node *ast, t_exec *exec);

// tokenize_2.0.c //
//t_token	*tokenize_input(char *input);
t_token	*tokenize_input(char *input, char **env);
int	get_word_length(char *str);
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **list, t_token *new);
char *extract_quoted_token(char *str, int *i, char quote);
void print_token_info(t_token *token);

// signals.c //
void    setup_signals(void);
void    reset_signals(void);
void    ignore_signals(void);

#endif