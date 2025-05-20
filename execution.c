/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:15:02 by gtretiak          #+#    #+#             */
/*   Updated: 2025/05/20 12:39:32 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_env_path(t_mini *ms)
{
	t_env	*current;
	char	**paths;

	current = ms->export;
	while (current)
	{
		if (strcmp(current->var, "PATH") == 0)
			break ;
		current = current->next;
	}
	if (!current)
		return (NULL);
	paths = ft_split(current->content + 5, ':');
	return (paths);
}

char	*find_command_path(const char *cmd, t_mini *ms)
{
	char	*prepath;
	char	*pathname;
	char	**env_paths;
	int			i;

	i = 0;
	env_paths = get_env_path(ms);
	if (!env_paths)
		return (NULL);
	while (env_paths[i])
	{
		prepath = ft_strjoin(env_paths[i++], "/");
		pathname = ft_strjoin(prepath, cmd);
		free(prepath);
		if (access(pathname, F_OK) == 0)
		{
			free_args_array(env_paths);
			return (pathname);
		}
		free(pathname);
	}
	free_args_array(env_paths);
	return (NULL);
}

static void	execute_cmd(t_token *cmd, t_mini *ms)
{
	char	*cmd_path;
	char	**env_array;

	reset_signals();
	env_array = env_to_array(ms->export);
	if (!env_array)
		exit(1);
	if (access(cmd->cmd, F_OK != 0) || ft_strncmp(cmd->cmd, "/", 1))
		cmd_path = find_command_path(cmd->cmd, ms);
	else
		cmd_path = cmd->cmd;
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_env_array(env_array);
		exit(127);
	}
	execve(cmd_path, cmd->args, env_array);
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": permission denied\n", 2);
		ms->exit_status = 126;
		exit(126);
	}
	else
	{
		perror("minishell");
		ms->exit_status = 2;
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
	free_env_array(env_array);
}

int	ft_execute_child(t_token *cmd, t_mini *ms)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_cmd(cmd, ms);
	return (pid);
}

int	ft_execute_parent(t_mini *ms, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	setup_signals();
//	if (WIFEXITED(status) || WIFSIGNALED(status))
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

	if (!cmd || !cmd->cmd || !cmd->args)
		return (-1);
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
