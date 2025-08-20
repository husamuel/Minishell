/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:51:12 by husamuel          #+#    #+#             */
/*   Updated: 2025/06/10 22:26:48 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

int	execute_simple_command(t_token *token, t_mini *ms)
{
	if (!is_valid_command(token, ms))
	{
		g_exit_status = 1;
		return (1);
	}
	if (token->type == CMD_BUILDIN)
	{
		g_exit_status = exec_builtin(token, ms);
		return (g_exit_status);
	}
	else if (token->type == CMD_EXPR)
	{
		g_exit_status = process_expr_command(token, ms);
		return (0);
	}
	return (0);
}

static void	setup_child_process(t_token *token, char *cmd_path, t_mini *ms)
{
	restore_child_tty();
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(token) == -1)
		exit(1);
	if (execve(cmd_path, token->args, get_env_array(ms->export)) == -1)
	{
		perror(token->cmd);
		free(cmd_path);
		exit(127);
	}
}

int	validate_and_find_command(t_token *token, t_mini *ms, char **cmd_path)
{
	if (!is_valid_command(token, ms))
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		return (127);
	}
	*cmd_path = find_command_path(token->cmd, ms->export);
	if (!*cmd_path)
	{
		print_command_not_found(token->cmd);
		return (127);
	}
	if (is_directory(*cmd_path))
	{
		ft_putstr_fd("minishell: is a directory\n", 2);
		free(*cmd_path);
		return (126);
	}
	return (0);
}

int	execute_fork_process(t_token *token, t_mini *ms, char *cmd_path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		setup_child_process(token, cmd_path, ms);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		setup_signals();
		disable_ctrl_backslash();
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (is_core_dumped(status));
		return (1);
	}
	else
		return (handle_fork_error2(cmd_path));
	return (0);
}
