/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:57:34 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/01 10:48:46 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		return (NULL);
	}
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	free(s1);
	return (result);
}

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
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
	char	*result;

	temp = ft_strjoin(current->var, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin_free(temp, current->content);
	return (result);
}

char	**get_env_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;

	count = count_env_vars(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = create_env_string(current);
		if (!env_array[i])
		{
			ft_free_split(env_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
