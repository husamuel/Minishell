/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:02:31 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/10 22:25:46 by husamuel         ###   ########.fr       */
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

static int	check_edge_syntax_errors(t_token *token)
{
	t_token	*last;

	if (token->type == CMD_PIPE || token->type == CMD_REDIRECT_IN
		|| token->type == CMD_REDIRECT_OUT || token->type == CMD_APPEND
		|| token->type == CMD_HEREDOC)
	{
		if (!token->prev || token->prev->type == CMD_NONE)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (0);
		}
	}
	last = token;
	while (last->next)
		last = last->next;
	if (last->type == CMD_PIPE || last->type == CMD_REDIRECT_IN
		|| last->type == CMD_REDIRECT_OUT || last->type == CMD_APPEND
		|| last->type == CMD_HEREDOC)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}

int	is_valid_command(t_token *token, t_mini *ms)
{
	(void)ms;
	if (!token || !token->cmd || token->type == CMD_NONE)
		return (0);
	if (token->is_invalid || !token->args || !token->args[0])
		return (0);
	if (!check_edge_syntax_errors(token))
		return (0);
	return (1);
}
