/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_updates.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:39:48 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/21 13:01:24 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_exec_signals(void);

void	disable_ctrl_backslash(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	ft_execute_child(t_token *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		reset_signals();
		execvp(cmd->cmd, cmd->args);
		exit(127);
	}
	return (pid);
}

int	ft_execute_parent(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
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

int	handle_fork_error(int stdout_backup)
{
	perror("minishell: fork");
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	setup_signals();
	return (1);
}

int	execute_command(t_token *cmd, t_mini *ms)
{
	pid_t	pid;
	int		stdout_backup;
	t_token	*current;
	int		status;

	(void)ms;
	current = cmd;
	if (!cmd || !cmd->cmd || !cmd->args)
		return (-1);
	stdout_backup = dup(STDOUT_FILENO);
	while (current && current->type != CMD_PIPE)
	{
		if (current->type == CMD_REDIRECT_OUT && current->next
			&& current->next->type == CMD_ARG_FILE)
		{
			if (handle_redirect_out(current->next, stdout_backup) != 0)
				return (1);
			current = current->next->next;
			continue ;
		}
		current = current->next;
	}
	setup_exec_signals();
	pid = ft_execute_child(cmd);
	if (pid <= 0)
		return (handle_fork_error(stdout_backup));
	status = ft_execute_parent(pid);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	return (status);
}