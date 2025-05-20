/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_execution2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husamuel <husamuel@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:25:38 by husamuel          #+#    #+#             */
/*   Updated: 2025/05/20 18:27:32 by husamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	handle_execve_with_cmd(t_token *current, t_mini *ms, char **envp)
{
	char	*path;
	char	*argv[2];

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
	handle_execve_error(path, envp);
}

void	handle_execve_error(char *path, char **envp)
{
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

void	execute_child_process(t_token *current, t_mini *ms)
{
	int		is_special;
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
		handle_execve_with_args(current, ms, envp);
	else if (current->cmd)
		handle_execve_with_cmd(current, ms, envp);
	else
	{
		free_env_array(envp);
		exit(127);
	}
}

void	handle_child_process(t_token *current, t_mini *ms, t_pipe_ctx *ctx)
{
	set_child_pipes(ctx->pipe_fds, ctx->cmd_index, ctx->count);
	execute_child_process(current, ms);
}
