/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:10:07 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/23 19:12:05 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	free_node(t_env *temp);
void	delete_node(t_env *prev, t_env *next);
void	unset_env(t_token *next, t_env **head);

void	exec_unset(t_token *token, t_mini *mini)
{
	t_token	*next;

	next = token->next;
	if (!next)
		return ;
	while (next)
	{
		unset_env(next, &mini->export);
		next = next->next;
	}
}

void	unset_env(t_token *next, t_env **head)
{
	t_env	*ev;
	t_env	*temp;

	if (!head || !*head || !next || !next->cmd)
		return ;
	ev = *head;
	if (ft_strcmp(ev->var, next->cmd) == 0)
	{
		*head = ev->next;
		if (*head)
			(*head)->prev = NULL;
		free_node(ev);
		return ;
	}
	while (ev && ev->next)
	{
		if (ft_strcmp(ev->next->var, next->cmd) == 0)
		{
			temp = ev->next;
			ev->next = temp->next;
			if (temp->next)
				temp->next->prev = ev;
			free_node(temp);
			return ;
		}
		ev = ev->next;
	}
}

void	delete_node(t_env *prev, t_env *next)
{
	if (prev && next)
	{
		prev->next = next;
		next->prev = prev;
	}
	else if (!prev)
		next->prev = NULL;
	else if (!next)
		prev->next = NULL;
}

void	free_node(t_env *temp)
{
	if (temp->var)
	{
		free(temp->var);
		temp->var = NULL;
	}
	if (temp->content)
	{
		free(temp->content);
		temp->content = NULL;
	}
	free(temp);
}
