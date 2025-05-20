/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child_executor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:24:08 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 17:24:09 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../minishell.h"

static int	process_redirects_before(t_token *temp, t_token *cmd_token,
			t_mini *ms, int *has_output_redirect)
{
	while (temp && temp != cmd_token)
	{
		if (temp->type == CMD_REDIRECT_OUT || temp->type == CMD_REDIRECT_IN
			|| temp->type == CMD_HEREDOC)
		{
			ms->exit_status = process_special_token2(temp, ms,
					has_output_redirect);
			if (ms->exit_status != 0)
			{
				return (ms->exit_status);
			}
			if (temp->next)
				temp = temp->next->next;
			else
				temp = NULL;
		}
		else
			temp = temp->next;
	}
	return (0);
}

static int	process_redirects_after(t_token *temp, t_mini *ms,
			int *has_output_redirect)
{
	while (temp && temp->type != CMD_PIPE)
	{
		if (temp->type == CMD_REDIRECT_OUT || temp->type == CMD_REDIRECT_IN
			|| temp->type == CMD_HEREDOC)
		{
			ms->exit_status = process_special_token2(temp, ms,
					has_output_redirect);
			if (ms->exit_status != 0)
			{
				return (ms->exit_status);
			}
			if (temp->next)
				temp = temp->next->next;
			else
				temp = NULL;
		}
		else
			temp = temp->next;
	}
	return (0);
}

static void	exit_child_error(t_pipe_ctx *ctx, int exit_code,
			int has_output_redirect)
{
	setup_child_pipes(ctx->pipe_fds, ctx->cmd_index, ctx->count,
		has_output_redirect);
	close_pipes(ctx);
	exit(exit_code);
}

void	execute_child(t_token *start_token, t_mini *ms, t_pipe_ctx *ctx)
{
	t_token	*cmd_token;
	t_token	*temp;
	int		has_output_redirect;
	int		status;

	has_output_redirect = 0;
	cmd_token = find_command(start_token);
	if (!cmd_token || !cmd_token->cmd)
	{
		ft_putstr_fd("minishell: invalid command\n", 2);
		exit_child_error(ctx, 127, has_output_redirect);
	}
	temp = start_token;
	status = process_redirects_before(temp, cmd_token, ms,
			&has_output_redirect);
	if (status != 0)
		exit_child_error(ctx, status, has_output_redirect);
	execute_child_part2(cmd_token, ms, ctx, has_output_redirect);
}

void	execute_child_part2(t_token *cmd_token, t_mini *ms,
			t_pipe_ctx *ctx, int has_output_redirect)
{
	t_token	*temp;
	int		status;

	temp = cmd_token->next;
	status = process_redirects_after(temp, ms, &has_output_redirect);
	if (status != 0)
		exit_child_error(ctx, status, has_output_redirect);
	setup_child_pipes(ctx->pipe_fds, ctx->cmd_index, ctx->count,
		has_output_redirect);
	build_args_from_tokens(cmd_token);
	execute_child_process(cmd_token, ms);
	close_pipes(ctx);
	exit(ms->exit_status);
}
