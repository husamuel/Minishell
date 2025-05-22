/* ************************************************************************** */
/*                                                                            */
/*                            :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:40:00 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/22 17:45:00 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void execute_pipeline_command(t_token *token_list, t_pipe_ctx *ctx, t_mini *ms, int type)
{
	char	*cmd_path;

	if (!token_list || !is_valid_command(token_list))
		exit(1);

	// Redireciona input do comando anterior
	if (ctx->cmd_index > 0)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 in");
			exit(1);
		}
	}
	// Redireciona output para o próximo comando
	if (ctx->cmd_index < ctx->count - 1)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 out");
			exit(1);
		}
	}
	close_all_pipes(ctx);

	if (type == CMD_BUILDIN)
		exit(exec_builtin(token_list, ms));
	else if (type == CMD_EXPR)
	{
		process_expr_command(token_list, ms);
		exit(0);
	}
	else if (type == CMD_EXEC)
	{
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
	exit(0);
}

int	execute_pipeline(t_mini *ms)
{
	t_pipe_ctx	ctx;
	pid_t		*pids;
	int			status;
	int			last_status = 0;
	int			i;
	char		**pipes;

	if (!ms || !ms->token || !ms->token->pipes_cmd)
		return (1);

	pipes = ms->token->pipes_cmd;
	for (ctx.count = 0; pipes[ctx.count]; ctx.count++);

	ctx.pipe_fds = create_pipes(ctx.count);
	if (!ctx.pipe_fds && ctx.count > 1)
		return (perror("pipe creation"), 1);

	pids = malloc(sizeof(pid_t) * ctx.count);
	if (!pids)
		return (close_all_pipes(&ctx), 1);

	// Inicializa ponteiro para tokens
	t_token *token_ptr = ms->token;

	i = -1;
	while (++i < ctx.count)
	{
		ctx.cmd_index = i;

		// Guarda o token atual (início do comando i)
		t_token *current_cmd_token = token_ptr;

		// Avança até ao próximo pipe (fim do comando atual)
		while (token_ptr && token_ptr->type != CMD_PIPE)
			token_ptr = token_ptr->next;
		// Salta o pipe para o próximo comando
		if (token_ptr && token_ptr->type == CMD_PIPE)
			token_ptr = token_ptr->next;

		pids[i] = fork();
		if (pids[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			if (ctx.cmd_index == ctx.count - 1)
			{
				if (setup_redirections(current_cmd_token) == -1)
					exit(1);
			}

			execute_pipeline_command(current_cmd_token, &ctx, ms, current_cmd_token->type);
		}
		else if (pids[i] < 0)
		{
			perror("fork");
			last_status = 1;
			break ;
		}
	}

	close_all_pipes(&ctx);

	i = -1;
	while (++i < ctx.count)
	{
		waitpid(pids[i], &status, 0);
		if (i == ctx.count - 1)
			last_status = WEXITSTATUS(status);
	}

	free(pids);
	return (last_status);
}
