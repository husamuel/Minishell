/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:15:02 by gtretiak          #+#    #+#             */
/*   Updated: 2025/04/26 14:07:28 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_path_dir(char *path, const char *cmd)
{
	char	*dir;
	char	*full_path;
	size_t	len;

	dir = strtok(path, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path);
			return (NULL);
		}
		snprintf(full_path, len, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

static char	*get_env_path(t_mini *ms)
{
	t_env	*current;

	current = ms->export;
	while (current)
	{
		if (strcmp(current->var, "PATH") == 0)
			return (current->content);
		current = current->next;
	}
	return (NULL);
}

char	*find_command_path(const char *cmd, t_mini *ms)
{
	char	*path;
	char	*env_path;

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	env_path = get_env_path(ms);
	if (!env_path || !*env_path)
		return (NULL);
	path = strdup(env_path);
	if (!path)
		return (NULL);
	return (find_in_path_dir(path, cmd));
}

static void	execute_cmd(t_token *cmd, t_mini *ms)
{
	char	*cmd_path;
	char	**env_array;

	reset_signals();
	env_array = env_to_array(ms->export);
	if (!env_array)
	{
		fprintf(stderr, "minishell: failed to create environment\n");
		exit(1);
	}
	cmd_path = find_command_path(cmd->cmd, ms);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->cmd);
		free_env_array(env_array);
		exit(127);
	}
	execve(cmd_path, cmd->args, env_array);
	fprintf(stderr, "minishell: %s: %s\n", cmd->cmd, strerror(errno));
	free(cmd_path);
	free_env_array(env_array);
	exit(127);
}

int	ft_execute_child(t_token *cmd, t_mini *ms)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_cmd(cmd, ms);
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
	status = ft_execute_parent(pid);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	return (status);
}