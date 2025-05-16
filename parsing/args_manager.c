/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:39:54 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/27 17:41:18 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	count_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
			i++;
	}
	return (i);
}

void	add_to_args(t_token *token, char *arg)
{
	int		i;
	char	**new_args;
	char	*new_str;

	new_str = ft_strdup(arg);
	if (!new_str)
		return ;
	i = count_args(token->args);
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free(new_str);
		return ;
	}
	new_args[i + 1] = NULL;
	new_args[i] = NULL;
	if (token->args)
	{
		i = 0;
		while (token->args[i])
		{
			new_args[i] = token->args[i];
			i++;
		}
		free(token->args);
	}
	new_args[i] = new_str;
	token->args = new_args;
}

void	add_to_args_file(t_token *token, char *arg)
{
	int		i;
	char	**new_args;
	char	*new_str;

	new_str = ft_strdup(arg);
	if (!new_str)
		return ;
	i = count_args(token->args_file);
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free(new_str);
		return ;
	}
	new_args[i + 1] = NULL;
	new_args[i] = NULL;
	if (token->args_file)
	{
		i = 0;
		while (token->args_file[i])
		{
			new_args[i] = token->args_file[i];
			i++;
		}
		free(token->args_file);
	}
	new_args[i] = new_str;
	token->args_file = new_args;
}
