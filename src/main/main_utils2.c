/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:59:02 by sinawara          #+#    #+#             */
/*   Updated: 2025/01/30 09:57:26 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	get_quote_type(char **res, int i)
{
	char	quote_type;

	quote_type = 0;
	if (res[i][0] == res[i][ft_strlen(res[i]) - 1] && (res[i][0] == 34
			|| res[i][0] == 39))
		quote_type = res[i][0];
	return (quote_type);
}

void	free_token_list_array(t_token *tokens, char **array)
{
	free_token_list(tokens);
	free_array(array);
}

char	**init_main(char **original_env)
{
	char	**new_env;

	new_env = init_env(original_env);
	setup_signals();
	return (new_env);
}

int	security_signal(char *prompt, char **new_env)
{
	reset_signals();
	parse_prompt(prompt, new_env);
	setup_signals();
	return (0);
}

void	init_parse_data(t_exec *exec, char **env, t_token **tokens,
		t_ast_node **ast)
{
	*tokens = NULL;
	*ast = NULL;
	ft_memset(exec, 0, sizeof(t_exec));
	exec->env = env;
}
