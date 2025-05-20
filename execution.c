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
	int		i;

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

static void	print_command_error(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

static void	handle_execve_error2(char *cmd, t_mini *ms)
{
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": permission denied\n", 2);
		ms->exit_status = 126;
		exit(126);
	}
	perror("minishell");
	ms->exit_status = 2;
	exit(EXIT_FAILURE);
}

void	execute_cmd(t_token *cmd, t_mini *ms)
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
		print_command_error(cmd->cmd);
		free_env_array(env_array);
		exit(127);
	}
	execve(cmd_path, cmd->args, env_array);
	handle_execve_error2(cmd->cmd, ms);
}
