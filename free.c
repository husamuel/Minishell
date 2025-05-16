/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:26:21 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/02 17:13:51 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_args_array(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

void	ft_free_minishell(t_mini *ms, int code)
{
	t_token	*current;
	t_token	*next;

	if (!ms || !ms->state)
		return ;
	current = ms->state->curr;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_args_array(current->args);
		if (current->args_file)
			free_args_array(current->args_file);
		if (code == 100)
			current = next;
		else
			break ;
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

void	free_token_content(t_token *token)
{
	if (token->cmd)
	{
		free(token->cmd);
		token->cmd = NULL;
	}
	if (token->args_file)
		free_args_array(token->args_file);
	if (token->args)
		free_args_array(token->args);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free_token_content(token);
		free(token);
		token = tmp;
	}
}
