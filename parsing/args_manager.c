/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:39:54 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/20 18:54:46 by husamuel         ###   ########.fr       */
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

static char	**alloc_new_args(char *arg, int old_count)
{
	char	**new_args;
	char	*new_str;

	new_str = ft_strdup(arg);
	if (!new_str)
		return (NULL);
	new_args = malloc(sizeof(char *) * (old_count + 2));
	if (!new_args)
	{
		free(new_str);
		return (NULL);
	}
	new_args[old_count + 1] = NULL;
	new_args[old_count] = new_str;
	return (new_args);
}

static void	copy_and_free_old_args(char **old, char **new_args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_args[i] = old[i];
		i++;
	}
	free(old);
}

void	add_to_args(t_token *token, char *arg)
{
	int		count;
	char	**new_args;

	count = count_args(token->args);
	new_args = alloc_new_args(arg, count);
	if (!new_args)
		return ;
	if (token->args)
		copy_and_free_old_args(token->args, new_args, count);
	token->args = new_args;
}

void	add_to_args_file(t_token *token, char *arg)
{
	int		count;
	char	**new_args;

	count = count_args(token->args_file);
	new_args = alloc_new_args(arg, count);
	if (!new_args)
		return ;
	if (token->args_file)
		copy_and_free_old_args(token->args_file, new_args, count);
	token->args_file = new_args;
}
