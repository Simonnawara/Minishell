/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trouilla <trouilla@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:45:48 by trouilla          #+#    #+#             */
/*   Updated: 2025/01/28 13:15:55 by trouilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	add_new_var(char **env, char *name, char *new_value)
{
	char	*final_value;
	int		i;

	final_value = create_new_value(name, NULL, new_value, 0);
	if (!final_value)
		return (1);
	i = 0;
	while (env[i])
		i++;
	env[i] = final_value;
	env[i + 1] = NULL;
	return (0);
}

static int	update_env(char **env, char *var)
{
	char	*name;
	char	*new_value;
	int		is_append;
	int		ret;

	if (!env || !var)
		return (1);
	name = get_var_name(var);
	if (!name)
		return (1);
	is_append = is_append_operation(var);
	new_value = get_var_value(var);
	if (!new_value)
	{
		free(name);
		return (1);
	}
	if (find_env_var(env, name))
		ret = update_existing_var(env, name, new_value, is_append);
	else
		ret = add_new_var(env, name, new_value);
	free(name);
	free(new_value);
	return (ret);
}

static void	print_export_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		ft_printf("declare -x %s\n", env[i++]);
}

static int	handle_export_args(char **args, char **env)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			ft_printf("export: '%s': not a valid identifier\n", args[i]);
			status = 1;
		}
		else if (update_env(env, args[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

int	ft_export(char **args, t_exec *exec)
{
	if (!exec || !exec->env || !args)
		return (1);
	if (!args[1])
	{
		print_export_vars(exec->env);
		return (0);
	}
	return (handle_export_args(args, exec->env));
}
