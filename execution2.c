/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:32:43 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/21 08:45:46 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_parent(t_mini *ms, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
		printf("\n");
	if (WIFEXITED(status))
		ms->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->exit_status = WTERMSIG(status) + 128;
	return (ms->exit_status);
}

int	handle_redirect_out(t_token *file_token, int stdout_backup)
{
	int	fd;

	fd = open(file_token->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	loop(t_token *cmd, int *stdout_backup)
{
	t_token	*current;

	current = cmd;
	*stdout_backup = dup(STDOUT_FILENO);
	while (current && current->type != CMD_PIPE)
	{
		if (current->type == CMD_REDIRECT_OUT && current->next
			&& current->next->type == CMD_ARG_FILE)
		{
			if (handle_redirect_out(current->next, *stdout_backup) != 0)
				return (1);
			current = current->next->next;
			continue ;
		}
		current = current->next;
	}
	return (0);
}

int	handle_fork_error1(int stdout_backup)
{
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	return (1);
}

int	execute_command(t_token *cmd, t_mini *ms)
{
	pid_t	pid;
	int		stdout_backup;
	int		status;

	if ((!cmd || !cmd->cmd || !cmd->args))
	{
		return (-1);
	}
	if (loop(cmd, &stdout_backup) != 0)
		return (1);
	setup_exec_signals();
	pid = ft_execute_child(cmd, ms);
	if (pid <= 0)
		return (handle_fork_error1(stdout_backup));
	status = ft_execute_parent(ms, pid);
	if (dup2(stdout_backup, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		status = 1;
	}
	close(stdout_backup);
	return (status);
}
