/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:02:31 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/03 18:25:40 by gtretiak         ###   ########.fr       */
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

int	ft_validate_redirect(t_token *curr)
{
	if (!curr->prev || (curr->prev->type != CMD_EXEC
			&& curr->prev->type != CMD_BUILDIN
			&& curr->prev->type != CMD_ARG))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(curr->cmd, 2);
		ft_putstr_fd("'\n", 2);
		g_exit_status = 2;
		return (0);
	}
	if (!curr->next || curr->next->type != CMD_ARG_FILE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected"
			" token `newline'\n", 2);
		g_exit_status = 2;
		return (0);
	}
	return (1);
}

int	is_valid_command(t_token *token, t_mini *ms)
{
	t_token	*curr;

	(void)ms;
	if (!token || !token->cmd || token->type == CMD_NONE)
		return (0);
	if (token->is_invalid || !token->args || !token->args[0])
		return (0);
	curr = token;
	while (curr)
	{
		if (curr->type == CMD_REDIRECT_OUT || curr->type == CMD_REDIRECT_IN
			|| curr->type == CMD_APPEND || curr->type == CMD_HEREDOC
			|| curr->type == CMD_PIPE)
		{
			if (!ft_validate_redirect(curr))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}
