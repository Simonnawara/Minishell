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


// free.c //
void	*free_and_return(char **array, void *return_value);
void	free_array(char **array);

// path.c //
char	*get_path(char **env);
char	*build_path(char *cmd, char **env);

// quotes.c //
int count_quotes(char *word, char quote);
int get_quote_type(char start_quote, char end_quote);

#endif