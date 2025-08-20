/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_pipes_cmd2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:11:56 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/26 13:24:37 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**create_pipes_cmd_array(t_token *token, int count)
{
	char	**pipes_cmd;
	int		i;

	pipes_cmd = malloc(sizeof(char *) * (count + 1));
	if (!pipes_cmd)
		return (NULL);
	i = 0;
	while (token && i < count)
	{
		pipes_cmd[i] = join_command_segment(&token);
		i++;
	}
	pipes_cmd[i] = NULL;
	return (pipes_cmd);
}

static void	assign_to_all_tokens(t_token *head, char **pipes_cmd,
		int *pipes_type)
{
	while (head)
	{
		head->pipes_cmd = pipes_cmd;
		head->pipes_type = pipes_type;
		head = head->next;
	}
}

void	fill_pipes_cmd(t_token *token)
{
	t_token	*head;
	char	**pipes_cmd;
	int		*pipes_type;
	int		count;

	if (!token)
		return ;
	head = token;
	count = count_pipe_segments(token);
	pipes_cmd = create_pipes_cmd_array(token, count);
	if (!pipes_cmd)
		return ;
	token = head;
	pipes_type = extract_command_types(token, count);
	if (!pipes_type)
	{
		ft_free_cmds(pipes_cmd);
		return ;
	}
	assign_to_all_tokens(head, pipes_cmd, pipes_type);
}
