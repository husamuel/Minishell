/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_exec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:23:50 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 17:23:52 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

void	setup_child_pipes(int **pipe_fds, int cmd_index,
	int pipe_count, int has_output_redirect)
{
	if (!pipe_fds || pipe_count <= 0)
		exit(1);
	dup_pipe_ends(pipe_fds, cmd_index, pipe_count, has_output_redirect);
	close_all_pipe_fds(pipe_fds, pipe_count);
}

t_token	*find_command(t_token *current)
{
	t_token	*temp;

	temp = current;
	while (temp && temp->type != CMD_EXEC && temp->type != CMD_BUILDIN)
		temp = temp->next;
	return (temp);
}

int	process_special_token2(t_token *token, t_mini *ms,
			int *has_output_redirect)
{
	if (token->type == CMD_REDIRECT_OUT)
	{
		*has_output_redirect = 1;
		return (handle_redirect(token, ms));
	}
	else if (token->type == CMD_REDIRECT_IN)
	{
		return (handle_redirect(token, ms));
	}
	else if (token->type == CMD_HEREDOC)
	{
		return (setup_heredoc(token, ms));
	}
	return (0);
}
