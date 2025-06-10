/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:02:31 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/10 18:38:49 by husamuel         ###   ########.fr       */
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

int	is_valid_command(t_token *token, t_mini *ms)
{
	(void)ms;
	if (!token || !token->cmd || token->type == CMD_NONE)
		return (0);
	if (token->is_invalid || !token->args || !token->args[0])
		return (0);
	return (1);
}