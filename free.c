/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/17 16:15:00 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_minishell(t_mini *ms, int code)
{
	if (!ms)
		return ;
	if (ms->token)
	{
		free_tokens(ms->token);
		ms->token = NULL;
	}
	if (ms->input)
	{
		free(ms->input);
		ms->input = NULL;
	}
	if (ms->prompt)
	{
		free(ms->prompt);
		ms->prompt = NULL;
	}
	if (code != -1 && ms->export)
	{
		free_env_list(ms->export);
		ms->export = NULL;
	}
}

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		if (current->var)
			free(current->var);
		free(current);
		current = next;
	}
}

static int	is_shared_cmd(char *cmd, char **arr)
{
	int	i;

	if (!cmd || !arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		if (arr[i] == cmd)
			return (1);
		i++;
	}
	return (0);
}

static void	free_args_array2(char **arr, char *shared)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		if (arr[i] != shared)
			free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_token_content(t_token *token)
{
	int		shared;

	if (!token)
		return ;
	shared = is_shared_cmd(token->cmd, token->args)
		|| is_shared_cmd(token->cmd, token->args_file);
	free_args_array2(token->args_file, token->cmd);
	free_args_array2(token->args, token->cmd);
	token->args = NULL;
	token->args_file = NULL;
	if (token->cmd && !shared)
		free(token->cmd);
	token->cmd = NULL;
}
