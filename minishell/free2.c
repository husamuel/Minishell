/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:26:24 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/08 12:26:02 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
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

char	*expand_and_free(char *str, t_mini *mini)
{
	char	*tmp;

	tmp = export_expand(str, mini);
	free(str);
	return (tmp);
}

void	ft_free_pipes_data(t_token *head)
{
	t_token	*current;

	if (!head || !head->pipes_cmd || !head->pipes_type)
		return ;
	ft_free_cmds(head->pipes_cmd);
	free(head->pipes_type);
	current = head;
	while (current)
	{
		current->pipes_cmd = NULL;
		current->pipes_type = NULL;
		current = current->next;
	}
}

void	ft_free_cmds(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
