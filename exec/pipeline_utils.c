#include "./../minishell.h"

int	count_commands(t_token *tokens)
{
	t_token	*current;
	int		count;

	current = tokens;
	count = 0;
	while (current)
	{
		if (is_valid_command(current))
			count++;
		current = current->next;
	}
	return (count);
}

int	**create_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	close_all_pipes(t_pipe_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->count - 1)
	{
		if (ctx->pipe_fds[i][0] != -1)
			close(ctx->pipe_fds[i][0]);
		if (ctx->pipe_fds[i][1] != -1)
			close(ctx->pipe_fds[i][1]);
		i++;
	}
}
