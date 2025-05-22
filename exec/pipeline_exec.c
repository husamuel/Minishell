#include "./../minishell.h"

static void	execute_pipeline_command(t_token *token, t_pipe_ctx *ctx, t_mini *ms)
{
	char	*cmd_path;

	if (ctx->cmd_index > 0)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 pipe input");
			exit(1);
		}
	}
	if (ctx->cmd_index < ctx->count - 1)
	{
		if (dup2(ctx->pipe_fds[ctx->cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 pipe output");
			exit(1);
		}
	}
	close_all_pipes(ctx);
	if (setup_redirections(token) == -1)
		exit(1);
	if (token->type == CMD_BUILDIN)
		exit(exec_builtin(token, ms));
	else if (token->type == CMD_EXPR)
	{
		process_expr_command(token, ms);
		exit(0);
	}
	else if (token->type == CMD_EXEC)
	{
		cmd_path = find_command_path(token->cmd, ms->export);
		if (!cmd_path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(token->cmd, 2);
			ft_putstr_fd("\n", 2);
			exit(127);
		}
		if (execve(cmd_path, token->args, get_env_array(ms->export)) == -1)
		{
			perror(token->cmd);
			free(cmd_path);
			exit(127);
		}
	}
	exit(0);
}

int	execute_pipeline(t_mini *ms)
{
	t_token		*current;
	t_pipe_ctx	ctx;
	pid_t		*pids;
	int			status;
	int			last_status;
	int			i;

	current = ms->token;
	ctx.count = count_commands(ms->token);
	ctx.pipe_fds = create_pipes(ctx.count);
	if (!ctx.pipe_fds && ctx.count > 1)
		return (perror("pipe creation"), 1);
	if (ctx.count <= 1)
		return (execute_token(current, ms));
	pids = malloc(sizeof(pid_t) * ctx.count);
	if (!pids)
		return (close_all_pipes(&ctx), 1);
	ctx.cmd_index = 0;
	while (current && ctx.cmd_index < ctx.count)
	{
		if (is_valid_command(current))
		{
			pids[ctx.cmd_index] = fork();
			if (pids[ctx.cmd_index] == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				execute_pipeline_command(current, &ctx, ms);
			}
			else if (pids[ctx.cmd_index] < 0)
			{
				perror("fork");
				last_status = 1;
				break ;
			}
			ctx.cmd_index++;
		}
		current = current->next;
	}
	i = -1;
	while (++i < ctx.count - 1)
	{
		close(ctx.pipe_fds[i][0]);
		close(ctx.pipe_fds[i][1]);
		free(ctx.pipe_fds[i]);
	}
	free(ctx.pipe_fds);
	i = -1;
	while (++i < ctx.cmd_index)
	{
		waitpid(pids[i], &status, 0);
		if (i == ctx.cmd_index - 1)
			last_status = WEXITSTATUS(status);
	}
	free(pids);
	return (last_status);
}
