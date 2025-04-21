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

int	execute_command(t_token *cmd, t_mini *ms)
{
	pid_t	pid;

	(void)ms;
	if (!cmd || !cmd->cmd || !cmd->args)
		return (-1);
	setup_exec_signals();
	pid = ft_execute_child(cmd);
	if (pid > 0)
		return (ft_execute_parent(pid));
	else
	{
		perror("minishell: fork");
		setup_signals();
		exit(1);
	}
}
