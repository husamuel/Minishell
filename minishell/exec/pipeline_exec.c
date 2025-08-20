/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:40:00 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/22 17:45:00 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	handle_pipe_redirection(t_pipe_ctx *ctx)
{
	if (ctx->cmd_index > 0)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 in");
			exit(1);
		}
	}
	if (ctx->cmd_index < ctx->count - 1)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			exit(1);
		}
	}
}

static void	handle_exec_command(t_token *token_list, t_mini *ms)
{
	char	*cmd_path;

	cmd_path = find_command_path(token_list->cmd, ms->export);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(token_list->cmd, 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	if (execve(cmd_path, token_list->args, get_env_array(ms->export)) == -1)
	{
		perror(token_list->cmd);
		free(cmd_path);
		exit(127);
	}
}

void	execute_pipeline_command(t_token *token_list, t_pipe_ctx *ctx,
		t_mini *ms, int type)
{
	if (!token_list || !is_valid_command(token_list, ms))
		exit(1);
	handle_pipe_redirection(ctx);
	close_all_pipes(ctx);
	if (type == CMD_BUILDIN)
		exit(exec_builtin(token_list, ms));
	else if (type == CMD_EXPR)
	{
		process_expr_command(token_list, ms);
		exit(0);
	}
	else if (type == CMD_EXEC)
		handle_exec_command(token_list, ms);
	exit(0);
}

int	init_pipeline_context(t_mini *ms, t_pipe_ctx *ctx, pid_t **pids)
{
	char	**pipes;

	if (!ms || !ms->token || !ms->token->pipes_cmd)
		return (1);
	pipes = ms->token->pipes_cmd;
	ctx->count = 0;
	while (pipes[ctx->count])
		ctx->count++;
	ctx->pipe_fds = create_pipes(ctx->count);
	if (!ctx->pipe_fds && ctx->count > 1)
		return (perror("pipe creation"), 1);
	*pids = malloc(sizeof(pid_t) * ctx->count);
	if (!*pids)
		return (close_all_pipes(ctx), 1);
	return (0);
}

t_token	*get_next_command_token(t_token **token_ptr)
{
	t_token	*current_cmd_token;

	current_cmd_token = *token_ptr;
	while (*token_ptr && (*token_ptr)->type != CMD_PIPE)
		*token_ptr = (*token_ptr)->next;
	if (*token_ptr && (*token_ptr)->type == CMD_PIPE)
		*token_ptr = (*token_ptr)->next;
	return (current_cmd_token);
}
