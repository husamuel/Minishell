/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:41:22 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/23 18:47:05 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	close_and_free_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
}

static void	free_pipes_on_error(int **pipe_fds, int index)
{
	int	j;

	j = 0;
	while (j <= index)
	{
		free(pipe_fds[j]);
		j++;
	}
	free(pipe_fds);
}

static int	**init_pipes(int pipe_count)
{
	int	**pipe_fds;
	int	i;

	pipe_fds = malloc(sizeof(int *) * pipe_count);
	if (!pipe_fds)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (!pipe_fds[i])
		{
			free_pipes_on_error(pipe_fds, i - 1);
			return (NULL);
		}
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("minishell: pipe");
			free_pipes_on_error(pipe_fds, i);
			return (NULL);
		}
		i++;
	}
	return (pipe_fds);
}

static void	set_child_pipes(int **pipe_fds, int cmd_index, int pipe_count)
{
	int	i;

	if (cmd_index > 0)
	{
		if (dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			exit(1);
		}
	}
	if (cmd_index < pipe_count)
	{
		if (dup2(pipe_fds[cmd_index][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			exit(1);
		}
	}
	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}

static void execute_child_process(t_token *current, t_mini *ms)
{
    char *argv[2];
    
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    
    if (current->type == CMD_BUILDIN)
        exit(exec_builtin(current, ms));
    else if (current->type == CMD_REDIRECT)
        exit(exec_redirect(current, ms));
    else if (current->type == CMD_HEREDOC)
        exit(exec_heredoc(current, ms));
    else
    {
        if (current->args && current->args[0])
        {
            execvp(current->args[0], current->args);
            perror("minishell: command not found");
        }
        else if (current->cmd)
        {
            argv[0] = current->cmd;
            argv[1] = NULL;
            execvp(current->cmd, argv);
            perror("minishell: command not found");
        }
        exit(127);
    }
}
static void	handle_child_process(t_token *current, t_mini *ms,
		int **pipe_fds, int cmd_index, int pipe_count)
{
	set_child_pipes(pipe_fds, cmd_index, pipe_count);
	execute_child_process(current, ms);
}

static t_token	*skip_pipe_tokens(t_token *current)
{
	while (current && current->type == CMD_PIPE)
		current = current->next;
	return (current);
}

static int	update_exit_status(t_mini *ms, int status)
{
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			ms->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ms->exit_status = WTERMSIG(status) + 128;
	}
	return (status);
}

int	exec_pipe(t_mini *ms)
{
	int		status;
	pid_t	pid;
	int		**pipe_fds;
	t_token	*current;
	int		cmd_index;
	int		pipe_count;

	status = 0;
	current = ms->token;
	pipe_count = ms->pipe;
	pipe_fds = init_pipes(pipe_count);
	if (!pipe_fds)
		return (1);
	cmd_index = 0;
	while (current)
	{
		if (current->type == CMD_PIPE || current->type == CMD_ARG)
		{
			current = current->next;
			continue ;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			close_and_free_pipes(pipe_fds, pipe_count);
			return (127);
		}
		if (pid == 0)
			handle_child_process(current, ms, pipe_fds, cmd_index, pipe_count);
		current = skip_pipe_tokens(current->next);
		cmd_index++;
	}
	close_and_free_pipes(pipe_fds, pipe_count);
	return (update_exit_status(ms, status));
}
