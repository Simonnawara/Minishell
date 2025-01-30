/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:55:05 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/30 16:02:49 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_no_value(char **env, char *name)
{
	int	ret;

	if (!find_env_var(env, name))
		ret = add_new_var(env, name, NULL);
	else
		ret = 0;
	free(name);
	return (ret);
}

int	handle_value(char **env, char *name, char *new_value, int is_append)
{
	int	ret;

	if (find_env_var(env, name))
		ret = update_existing_var(env, name, new_value, is_append);
	else
		ret = add_new_var(env, name, new_value);
	free(name);
	free(new_value);
	return (ret);
}
