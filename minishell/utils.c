/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:05:22 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/01 10:18:52 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_zero(t_mini *ms)
{
	free(ms->input);
	free_tokens(ms->token);
	printf("minishell: erro de sintaxe\n");
	exit(EXIT_FAILURE);
}

static int	count_env_vars(t_env *env)
{
	t_env	*current;
	int		count;

	current = env;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *current)
{
	char	*temp;
	char	*res;

	temp = ft_strjoin(current->var, "=");
	if (!temp)
		return (NULL);
	if (current->content)
	{
		res = ft_strjoin(temp, current->content);
		free(temp);
		if (!res)
			return (NULL);
		return (res);
	}
	return (temp);
}

static void	free_partial_env_array(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(env_array[i++]);
	free(env_array);
}

char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = count_env_vars(env);
	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			free_partial_env_array(env_array, i);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
