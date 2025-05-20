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

static void	setup_pipe_dup(int **pipe_fds, int cmd_index, int pipe_count)
{
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
}

static void	close_all_child_pipes(int **pipe_fds, int pipe_count)
{
	int	i;

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

void	set_child_pipes(int **pipe_fds, int cmd_index, int pipe_count)
{
	if (!pipe_fds || pipe_count <= 0)
		exit(1);
	setup_pipe_dup(pipe_fds, cmd_index, pipe_count);
	close_all_child_pipes(pipe_fds, pipe_count);
}

int	ft_execute_special_in_child(t_token *current, t_mini *ms)
{
	if (current->type == CMD_BUILDIN)
		return (exec_builtin(current, ms));
	else if (current->type == CMD_REDIRECT)
		return (exec_redirect(current, ms));
	else if (current->type == CMD_HEREDOC)
		return (exec_heredoc(current, ms));
	return (-1);
}

void	handle_execve_with_args(t_token *current, t_mini *ms, char **envp)
{
	char	*path;

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
	handle_execve_error(path, envp);
}
