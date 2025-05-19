/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_building_args.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:22:36 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/19 14:30:59 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	build_args_from_tokens(t_token *cmd)
{
	t_token	*current;
	int		count;
	int		i;

	if (!cmd)
		return ;

	if (cmd->args)
	{
		int j = 0;
		while (cmd->args[j])
			free(cmd->args[j++]);
		free(cmd->args);
		cmd->args = NULL;
	}
	current = cmd;
	count = 0;
	while (current && current->type != CMD_REDIRECT_OUT
		&& current->type != CMD_REDIRECT_IN
		&& current->type != CMD_HEREDOC && current->type != CMD_PIPE)
	{
		count++;
		current = current->next;
	}
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (!cmd->args)
		return ;

	current = cmd;
	i = 0;
	while (current && i < count)
	{
		cmd->args[i++] = current->cmd;
		current = current->next;
	}
	cmd->args[i] = NULL;
}
