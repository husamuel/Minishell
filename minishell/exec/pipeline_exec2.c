/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:07:21 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/01 10:13:40 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	fork_and_execute(t_pipe_ctx *ctx, t_mini *ms, pid_t *pids,
		t_token *current_cmd_token)
{
	pids[ctx->cmd_index] = fork();
	if (pids[ctx->cmd_index] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (setup_redirections(current_cmd_token) == -1)
			exit(1);
		execute_pipeline_command(current_cmd_token, ctx, ms,
			current_cmd_token->type);
	}
	else if (pids[ctx->cmd_index] < 0)
	{
		perror("fork");
		return (1);
	}
	return (0);
}

static int	wait_for_children(pid_t *pids, int count)
{
	int	status;
	int	last_status;
	int	i;

	last_status = 0;
	i = -1;
	while (++i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				return (is_core_dumped(status));
		}
	}
	return (last_status);
}

static int	execute_pipeline_loop(t_pipe_ctx *ctx, t_mini *ms, pid_t *pids)
{
	int		i;
	t_token	*token_ptr;
	t_token	*current_cmd_token;

	token_ptr = ms->token;
	i = -1;
	while (++i < ctx->count)
	{
		ctx->cmd_index = i;
		current_cmd_token = get_next_command_token(&token_ptr);
		if (fork_and_execute(ctx, ms, pids, current_cmd_token) != 0)
			return (1);
	}
	return (0);
}

int	execute_pipeline(t_mini *ms)
{
	t_pipe_ctx	ctx;
	pid_t		*pids;
	int			last_status;

	if (init_pipeline_context(ms, &ctx, &pids) != 0)
	{
		g_exit_status = 1;
		return (1);
	}
	if (execute_pipeline_loop(&ctx, ms, pids) != 0)
		last_status = 1;
	else
		last_status = 0;
	close_all_pipes(&ctx);
	if (last_status == 0)
		last_status = wait_for_children(pids, ctx.count);
	free(pids);
	g_exit_status = last_status;
	return (last_status);
}
