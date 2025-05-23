/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:02:31 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/23 18:06:33 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	if (!key || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (strcmp(current->var, key) == 0)
			return (ft_strdup(current->content));
		current = current->next;
	}
	return (NULL);
}
