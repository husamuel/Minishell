#include "./../minishell.h"

int	**init_pipes_alloc(int count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (!pipe_fds[i])
			return (NULL);
		i++;
	}
	return (pipe_fds);
}

int	**init_pipes1(int count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = init_pipes_alloc(count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (pipe(pipe_fds[i]) == -1)
		{
			while (--i >= 0)
			{
				close(pipe_fds[i][0]);
				close(pipe_fds[i][1]);
				free(pipe_fds[i]);
			}
			free(pipe_fds);
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

void	close_and_free_pipes1(t_pipe_ctx *ctx)
{
	int	i;

	if (!ctx || !ctx->pipe_fds)
		return ;
	i = 0;
	while (i < ctx->count)
	{
		if (ctx->pipe_fds[i])
		{
			close(ctx->pipe_fds[i][0]);
			close(ctx->pipe_fds[i][1]);
			free(ctx->pipe_fds[i]);
		}
		i++;
	}
	free(ctx->pipe_fds);
	ctx->pipe_fds = NULL;
}

static int	process_redirect_file(t_token *token, t_token *next)
{
	int	fd;

	fd = ft_open_redirect_file(token, next);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: error opening file\n", 2);
		return (1);
	}
	if (fd == -127)
		return (127);
	return (fd);
}

static int	setup_redirect_single(t_token *token, t_token *next, 
	t_token **after)
{
	int	fd;
	int	is_input;
	int	original_fd;

	is_input = (token->type == CMD_REDIRECT_IN);
	fd = process_redirect_file(token, next);
	if (fd <= 0)
		return (fd);
	original_fd = ft_redir_exec_setup(fd, is_input);
	*after = ft_unlink_tokens(token, next);
	if (is_input)
		dup2(original_fd, STDIN_FILENO);
	else
		dup2(original_fd, STDOUT_FILENO);
	close(original_fd);
	return (0);
}

static int	setup_redirects(t_token *token, t_token **next_cmd)
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

static void	prepare_child_io(t_pipe_ctx *ctx)
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

static void	close_all_pipes_in_child(t_pipe_ctx *ctx)
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

static void	close_pipe_ends(t_pipe_ctx *ctx)
{
	if (ctx->cmd_index > 0)
		close(ctx->pipe_fds[ctx->cmd_index - 1][0]);
	if (ctx->cmd_index < ctx->count)
		close(ctx->pipe_fds[ctx->cmd_index][1]);
}

static int	process_pipeline_segment(t_token **current, t_mini *ms, 
									t_pipe_ctx *ctx)
{
	pid_t	pid;
	int		redirect_status;
	t_token	*next_cmd;

	if (!current || !*current)
		return (1);
	redirect_status = setup_redirects(*current, &next_cmd);
	if (redirect_status != 0)
		return (redirect_status);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_and_free_pipes1(ctx);
		return (127);
	}
	if (pid == 0)
		handle_child_process1(*current, ms, ctx);
	close_pipe_ends(ctx);
	*current = next_cmd;
	return (0);
}

static void	process_wait_status(t_mini *ms)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ms->exit_status = WTERMSIG(status) + 128;
	}
}

static int	process_pipeline_tokens(t_mini *ms, t_pipe_ctx *ctx)
{
	t_token	*current;
	int		status;

	current = ms->token;
	while (current)
	{
		if (current->type == CMD_PIPE)
		{
			current = current->next;
			continue;
		}
		status = process_pipeline_segment(&current, ms, ctx);
		if (status != 0)
		{
			close_and_free_pipes1(ctx);
			return (status);
		}
		while (current && current->type != CMD_PIPE)
			current = current->next;
		ctx->cmd_index++;
	}
	return (0);
}

int	exec_pipe_with_redirects(t_mini *ms)
{
	t_pipe_ctx	ctx;
	int			status;

	if (!ms || !ms->token)
	{
		ft_putstr_fd("minishell: invalid input\n", 2);
		return (1);
	}
	ctx.cmd_index = 0;
	ctx.count = ms->pipe;
	ctx.pipe_fds = init_pipes1(ctx.count);
	if (!ctx.pipe_fds)
	{
		ft_putstr_fd("minishell: pipe initialization failed\n", 2);
		return (1);
	}
	status = process_pipeline_tokens(ms, &ctx);
	if (status != 0)
		return (status);
	close_and_free_pipes1(&ctx);
	process_wait_status(ms);
	return (ms->exit_status);
}