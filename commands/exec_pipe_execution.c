/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:34:46 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/19 17:44:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	set_child_pipes(int **pipe_fds, int cmd_index, int pipe_count)
{
	int		i;

	if (!pipe_fds || pipe_count <= 0)
		exit(1);
	if (cmd_index > 0)
	{
		if (cmd_index - 1 >= pipe_count || !pipe_fds[cmd_index - 1])
			exit(1);
		if (dup2(pipe_fds[cmd_index - 1][0], STDIN_FILENO) == -1)
			exit(1);
	}
	if (cmd_index < pipe_count)
	{
		if (cmd_index >= pipe_count || !pipe_fds[cmd_index])
			exit(1);
		if (dup2(pipe_fds[cmd_index][1], STDOUT_FILENO) == -1)
			exit(1);
	}
	i = 0;
	while (i < pipe_count)
	{
		if (pipe_fds[i])
		{
			close(pipe_fds[i][0]);
			close(pipe_fds[i][1]);
		}
		i++;
	}
}

static int	ft_execute_special_in_child(t_token *current, t_mini *ms)
{
	if (current->type == CMD_BUILDIN)
		return (exec_builtin(current, ms));
	else if (current->type == CMD_REDIRECT)
		return (exec_redirect(current, ms));
	else if (current->type == CMD_HEREDOC)
		return (exec_heredoc(current, ms));
	return (-1);
}

void	execute_child_process(t_token *current, t_mini *ms)
{
	char	*argv[2];
	int		is_special;
	char	*path;
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	is_special = ft_execute_special_in_child(current, ms);
	if (is_special != -1)
		exit(is_special);
	envp = env_to_array(ms->export);
	if (!envp)
		exit(1);
	if (current->args && current->args[0])
	{
		path = find_command_path(current->args[0], ms);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			free_env_array(envp);
			exit(127);
		}
		execve(path, current->args, envp);
	}
	else if	(current->cmd)
	{
		path = find_command_path(current->cmd, ms);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			free_env_array(envp);
			exit(127);
		}
		argv[0] = current->cmd;
		argv[1] = NULL;
		execve(path, argv, envp);
	}
	else
	{
		free_env_array(envp);
		exit(127);
	}
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": permission denied\n", 2);
		free(path);
		free_env_array(envp);
		exit(126);
	}
	perror("minishell");
	free(path);
	free_env_array(envp);
	exit(127);
}

void	handle_child_process(t_token *current, t_mini *ms, t_pipe_ctx *ctx)
{
	set_child_pipes(ctx->pipe_fds, ctx->cmd_index, ctx->count);
	execute_child_process(current, ms);
}
