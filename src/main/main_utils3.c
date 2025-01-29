/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 10:09:23 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/29 11:40:14 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_new_token(t_token **tokens, char *cmd, t_parse_data *data,
	char *res_i)
{
	t_token	*new_token;

	(void)res_i;
	data->type = classify_token_prev(cmd, data->env, data->prev_type);
	if (!ft_strcmp(cmd, "|") == 0)
		data->type = T_WORD;
	new_token = create_token(cmd, data->type);
	if (new_token && new_token->value && !ft_strcmp(new_token->value, "echo"))
	{
		new_token->res = data->res;
		new_token->echo_counter = data->i;
	}
	if (data->type == T_PIPE)
		data->exec->compteur_pipe++;
	if (!new_token)
		return (0);
	add_token(tokens, new_token);
	if (!*tokens)
		ft_putendl_fd("Error: Tokens list is NULL after add_token", 2);
	return (1);
}

int	handle_token_error(t_token **tokens, char **res, char *cmd)
{
	if (cmd)
		free(cmd);
	free_token_list(*tokens);
	free_array(res);
	return (1);
}

int	check_token_errors(t_token_type type, char **res, int i)
{
	return (check_pipe(type, res, i) || check_redirect(type, res, i));
}

void	cleanup_ast_resources(t_ast_node *ast, char **res)
{
	if (ast)
	{
		free_ast_node(ast);
		ast = NULL;
	}
	free_array(res);
}