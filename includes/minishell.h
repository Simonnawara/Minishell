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
# define MINISHEL_H

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


typedef enum e_token_type {
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

typedef struct s_token {
    char        *value;
    t_token_type type;
    struct s_token *next;
} t_token;

typedef struct s_command {
    char    *name;
    char    **args;
    char    *input_file;
    char    *output_file;
    int     append_output;
    struct s_command *next;  // For piped commands
} t_command;

// free.c //
void	*free_and_return(char **array, void *return_value);
void	free_array(char **array);
char	**free_split(char **res);

// path.c //
char	*get_path(char **env);
char	*build_path(char *cmd, char **env);

// quotes.c //
int count_quotes(char *word, char quote);
int get_quote_type(char start_quote, char end_quote);

// token_builder.c //
char **tokenize(char *prompt);

// token_list.c //
t_token_type	get_operator_type(char *token);
t_token_type	classify_token(char *token, char **env);

#endif