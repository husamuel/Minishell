/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_with_redirect2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:37:51 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:48:27 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	setup_redirects(t_token *token, t_token **next_cmd)
{
	t_token	*next;
	t_token	*after;
	int		status;
	int		redirect_result;

	status = 0;
	*next_cmd = token;
	while (token && (token->type == CMD_REDIRECT_IN
			|| token->type == CMD_REDIRECT_OUT))
	{
		next = token->next;
		if (!next || !next->cmd)
			return (-127);
		redirect_result = setup_redirect_single(token, next, &after);
		if (redirect_result != 0)
			return (redirect_result);
		*next_cmd = after;
		token = after;
	}
	return (status);
}

void	prepare_child_io(t_pipe_ctx *ctx)
{
	if (ctx->cmd_index > 0)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (ctx->cmd_index < ctx->count)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index][1], STDOUT_FILENO) == -1)
			exit(1);
	}
}

void	close_all_pipes_in_child(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->count)
	{
		close(ctx->pipe_fds[i][0]);
		close(ctx->pipe_fds[i][1]);
		i++;
	}
}

void	handle_child_process1(t_token *cmd, t_mini *ms, t_pipe_ctx *ctx)
{
	prepare_child_io(ctx);
	close_all_pipes_in_child(ctx);
	if (cmd && cmd->cmd && (cmd->type == CMD_EXEC || cmd->type == CMD_BUILDIN))
	{
		build_args_from_tokens(cmd);
		execute_command(cmd, ms);
	}
	else
	{
		ft_putstr_fd("minishell: invalid command\n", 2);
		exit(127);
	}
	close_and_free_pipes1(ctx);
	exit(ms->exit_status);
}

void	close_pipe_ends(t_pipe_ctx *ctx)
{
	if (ctx->cmd_index > 0)
		close(ctx->pipe_fds[ctx->cmd_index - 1][0]);
	if (ctx->cmd_index < ctx->count)
		close(ctx->pipe_fds[ctx->cmd_index][1]);
}
