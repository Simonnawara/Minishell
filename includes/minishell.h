/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 09:56:24 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/27 09:56:24 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include "../libft/include/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern int	g_exit_status;

typedef struct s_expand_data
{
	char	*result;
	size_t	total_len;
	int		j;
}	t_expand_data;

typedef struct s_path
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	char	*temp;
}	t_path;

typedef enum e_token_type
{
	T_WORD,
	T_COMMAND,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_PIPE,
	T_HEREDOC,
	T_SEMICOLON,
	T_PAREN_L,
	T_PAREN_R,
	T_BUILTIN,
	T_EXIT_STATUS
}	t_token_type;

typedef struct s_token
{
	char			*value;
	char			**res;
	int				echo_counter;
	int				is_echo;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_command_table
{
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
	char	*heredoc_file;
	char	*delimiter;
	char	**res;
	int		echo_counter;
	int		heredoc;
	int		append;
	int		saved_stdin;
	int		saved_stdout;
	int		pipe_out;
	char	**all_outfiles;
	int		num_outfiles;
}	t_command_table;

typedef struct s_ast_node
{
	t_token_type		type;
	char				*value;
	char				**args;
	char				**res;
	int					echo_counter;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

typedef struct s_exec
{
	t_ast_node	*ast;
	t_path		*path_info;
	int			last_status;
	int			pipe_read;
	int			pipe_write;
	char		**env;
	int			*env_allocated;
	int			compteur_pipe;
}	t_exec;

typedef struct s_heredoc
{
	char			*filename;
	int				fd;
}	t_heredoc;

typedef struct s_shell_data
{
	char			**env;
	char			*expanded_arg;
	t_token_type	prev_type;
	char			quote_type;
	int				quote_count;
	t_exec			*exec;
}	t_shell_data;

typedef struct s_quote_data
{
	int		i;
	char	**res;
}	t_quote_data;

// Function prototypes
void			init_shell_data(t_shell_data *data, char **env, t_exec *exec);
int				handle_unquoted_token(char *cmd, t_shell_data *data, int i);
void			cleanup_shell(char **new_env);
int				free_token_resources(t_token **tokens, t_token *new_token);

// main.c //
char			*get_command(char *word, int quote_count, char quote_type);
char			**init_env(char **original_env);
int				parse_prompt(char *prompt, char **env);
t_token			*process_unquoted_token(char **res, int i, char **env,
					t_token_type prev_type);
int				process_token(t_token **tokens, char **res, int i, char **env);
t_token			*process_quoted_token(char quote_type, char **env,
					t_token_type prev_type, t_quote_data *data);

// main_utils.c //
int				validate_inputs(int argc);
void			free_prompt_resources(t_token *tokens, char **res,
					char *expanded_arg);
char			*get_command(char *word, int quote_count, char quote_type);
int				check_exit_status(const char *str);

// main_utils2.c //
char			get_quote_type(char **res, int i);
void			free_token_list_array(t_token *tokens, char **array);
char			**init_main(char **original_env);
int				security_signal(char *prompt, char **new_env);
void			init_parse_data(t_exec *exec, char **env, t_token **tokens,
					t_ast_node **ast);

// main_utils3.c //
t_token_type	get_last_token_type(t_token *tokens);
char			*init_home_value(char **env);
int				handle_empty_cmd(char **cmd, char *home_value,
					char *full_path, char **env);

// main_utils4.c //
int				handle_exit_status(char *expanded_arg);
char			*expand_exit_status(char *processed_arg, int *stop_processing);
int				handle_expansion(char **cmd, char **processed_arg, char **env,
					int i);
int				handle_home_expansion(char **cmd, char **res, int i,
					char **env);
t_quote_data	*init_quote_data(char **res, int i);

// free.c //
void			free_array(char **array);
void			*free_and_return(char **array, void *return_value);
int				free_word_and_return(char *word, int return_value);
int				free_array_and_return(char **array, int return_value);
void			free_token_list(t_token *tokens);
void			free_ast_node(t_ast_node *node);
void			free_ast(t_ast_node *root);
void			free_command_table(t_command_table *cmd);
void			cleanup_token(t_token *token);

// errors.c //
void			file_error(char *filename);
int				is_command_found(char *word, char **env);
int				verify_forbidden_tokens(char *prompt);
int				ft_strcmp(const char *s1, const char *s2);
void			*ft_memcpy(void *dest, const void *src, size_t n);
char			*check_and_replace_exit_status(const char *str,
					int exit_status);
int				check_exit_status(const char *str);

// error_pipe.c //
int				pipe_error(void);
void			close_pipe(int pipe_fd[2]);
int				fork_error(void);

// path.c //
char			*get_path(char **env);
char			*build_path(char *cmd, char **env);

// quotes.c //
int				count_quotes(char *word, char quote);

// tokenize.c //
char			**tokenize(char *prompt);
int				count_words(const char *str);
int				move_past_quotes(const char *str, char quote_type, int *i);

// tokenize_utils.c //
int				is_operator(char c);
int				get_operator_len(const char *str);
int				handle_quoted_word(const char *str, int *len, int start);
int				handle_regular_word(const char *str, int *len, int start);
int				process_word_for_count(const char *str, int *i);

// tokenize_utils2.c //
char			**handle_tokenize_error(char **tokens, int token_idx);

// token_type.c //
t_token_type	get_operator_type(char *token);
t_token_type	classify_token(char *token, char **env);
t_token_type	classify_token_prev(char *token, char **env,
					t_token_type prev_type);
int				check_pipe(t_token_type type, char **res, int i);
int				check_redirect(t_token_type type, char **res, int i);

// token_type_utils.c //
t_token_type	check_executable(char *token);
int				check_consecutive_pipes(char **res);
int				check_pipe_position(char **res, int i, t_token_type type);
int				handle_redirect_error(char **res, int i);
int				open_redirect_file(char **res, int i, int type);

// build_ast_utils.c //
t_command_table	*init_command_table(void);
int				add_argument_to_command(t_ast_node *cmd_node, char *arg);
t_command_table	*convert_node_to_command(t_ast_node *node);
char			**copy_string_array(char **arr);

// build_ast.c //
t_ast_node		*create_ast_node(t_token_type type, char *value);
t_ast_node		*build_command_node(t_token **tokens);
t_ast_node		*build_ast(t_token **tokens);
void			print_ast(t_ast_node *root);
void			print_full_ast(t_ast_node *root);
t_ast_node		*handle_redirections(t_token **tokens);
t_ast_node		*handle_multiple_redirections(t_token **tokens);
t_ast_node		*handle_pipe_creation(t_token **tokens,
					t_token *current, t_token *split);
t_ast_node		*handle_redirection_sequence(t_token **tokens);
t_ast_node		*create_redirection_node(t_token *current,
					t_ast_node **root, t_ast_node *cmd_node);
void			handle_root_assignment(t_ast_node **root, t_ast_node *redir,
					t_ast_node *cmd_node);
void			handle_prev_redir(t_ast_node *prev_redir, t_ast_node *redir,
					t_ast_node *cmd_node);

// execute_ast.c && utils//
int				execute_ast(t_ast_node *ast, t_exec *exec);
int				execute_pipe_child(t_ast_node *ast, t_exec *exec,
					int *pipe_fds, int is_left);
int				handle_pipe(t_ast_node *ast, t_exec *exec);
void			cleanup_nodes(t_ast_node *root, t_ast_node *cmd_node,
					t_ast_node *redir);

// execute_pipe.c //
int				execute_pipe_node(t_ast_node *node, t_exec *exec);

// execute_logical_operator //
int				execute_logical_node(t_ast_node *node, t_exec *exec);

// exec_simple_command.c //
int				execute_simple_command(t_ast_node *node, t_exec *exec,
					t_command_table cmd);
int				is_builtin(char *cmd);

// Builtin *.c //
char			*expand_variables(char *str, char **env);
char			*get_env_value(char *var_name, char **env);
int				ft_pwd(void);
int				ft_export(char **args, t_exec *exec);
int				ft_unset(t_exec *exec, char **args);
int				ft_env(t_exec *exec, char **args);
int				ft_exit(t_ast_node *node, t_exec *exec);
int				ft_cd(char **args, t_exec *exec);

// echo.c //
int				process_echo_arg(char **res, int res_index, t_exec *exec,
					int *prev_was_quote);
int				is_valid_n_option(const char *str);
int				handle_echo_args(char **res, int res_index, t_exec *exec,
					int *prev_was_quote);
int				check_n_option(char **args, int *i, int *n_option);
int				ft_echo(char **args, char **res, int echo_counter,
					t_exec *exec);

// echo_utils1.c //
char			*get_env_value(char *var_name, char **env);
int				expand_var(char *str, int *i, char **env, t_expand_data *data);
char			*process_var(char *str, int *i, char **env, size_t *total_len);
int				calculate_total_len(char *str, char **env, size_t *total_len);
char			*expand_variables(char *str, char **env);

// echo_utils2.c //
char			*strip_quotes(const char *str);
void			cleanup_echo_resources(char *stripped_arg, char *processed_arg,
					char *expanded_arg);
void			handle_expanded_arg(char *expanded_arg, char *stripped_arg,
					char *processed_arg);
void			handle_processed_arg(char *processed_arg, char *stripped_arg);
void			handle_stripped_arg(char *stripped_arg);

// echo_utils3.c //
int				handle_exit_status_check(char *stripped_arg,
					char *processed_arg, char *expanded_arg);
int				handle_dollar_expansion(char *stripped_arg,
					char *processed_arg, t_exec *exec);
int				process_quoted_dollar(char *stripped_arg,
					char *processed_arg, t_exec *exec);
int				process_dollar_arg(char *stripped_arg, char *res_i,
					t_exec *exec, char *processed_arg);
int				check_space_conditions(char **res, int res_index,
					int *prev_was_quote);

// cd_utils.c //
void			print_error(char **args);
char			*get_env_value1(char **env, const char *var);
int				update_pwd_vars(t_exec *exec, const char *new_path);
char			*get_absolute_path(char *base, char *arg);
int				update_env_var(t_exec *exec, const char *name,
					const char *value);
int				init_export_args(char **args, const char *name,
					const char *value, char **var);

// export_utils.c && export_utils_more.c //
char			*find_env_var(char **env, char *name);
int				is_append_operation(char *var);
char			*create_new_value(char *name, char *old_value, char *new_value,
					int is_append);
char			*get_existing_value(char **env, char *name, int is_append);
int				update_existing_var(char **env, char *name, char *new_value,
					int is_append);
char			*get_var_value(char *var);
char			*get_var_name(char *var);
int				is_valid_id(char *str);
char			*append_values(char *name, char *old_value, char *new_value);

// redirection_input //
int				setup_redirection(t_command_table *cmd);
int				restore_io(t_command_table *cmd);

// set_env_cd.c //
int				update_env_variable(char **env, char *key, char *value);

// external command .c //
int				execute_external_command(t_command_table *cmd, t_exec *exec);

// extern_cmd_path //
char			*find_command_path(char *cmd, t_path *path_info);
//char			*search_in_path(t_path *path_info, char *cmd);

// Heredoc.c //
int				execute_heredoc(t_ast_node *ast, t_exec *exec);
int				collect_heredocs(t_ast_node *ast, t_heredoc **hds);
int				handle_heredoc_error(t_heredoc *new_hds, int i);
int				execute_command_node(t_ast_node *cmd_node, t_exec *exec,
					int last_heredoc_fd);
int				setup_input_redirection(t_command_table *cmd,
					t_ast_node *current, t_exec *exec);
int				handle_heredoc_error_2(t_heredoc *heredocs, int heredoc_count);

//heredoc_util.c //
char			*get_heredoc_filename(void);
int				write_to_heredoc(int fd, char *delimiter);
void			cleanup_heredoc(t_heredoc *hd);
int				setup_heredoc(char *delimiter, t_heredoc *hd);
int				count_heredocs(t_ast_node *ast);
void			cleanup_all_heredocs(t_heredoc *hds, int count);

// tokenize_2.0.c //
t_token			*tokenize_input(char *input, char **env);
int				get_word_length(char *str);
t_token			*create_token(char *value, t_token_type type);
void			add_token(t_token **list, t_token *new);
char			*extract_quoted_token(char *str, int *i, char quote);
void			print_token_info(t_token *token);

// signals.c //
void			setup_signals(void);
void			handle_sigint(int sig);
void			handle_heredoc_sigint(int sig);
void			setup_heredoc_signals(void);
void			reset_signals(void);
void			ignore_signals(void);
void			update_exit_status(int status);
char			*get_exit_status(t_exec *exec);
int				init_command_check(char *word, char **env, char *quote_type);

#endif