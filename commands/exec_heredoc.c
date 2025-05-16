#include "./../minishell.h"

static int	ft_handle_heredoc_input(const t_token *next, int fd[2])
{
	char	*line;

	if (!next || !next->cmd)
		return (127);
	if (pipe(fd) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
		return (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, next->cmd) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(fd[1]);
	return (0);
}

static int	ft_execute_builtin(t_token *cmd_token, t_mini *ms, int fd)
{
	int	original_stdin;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = exec_builtin(cmd_token, ms);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

static int	ft_execute_external(t_token *cmd_token, t_mini *ms, int fd)
{
	int	original_stdin;
	int	status;

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	status = execute_command(cmd_token, ms);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

static int	ft_heredoc_default(int fd)
{
	int		original_stdin;
	int		bytes_read;
	char	buffer[4096];

	original_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	while ((bytes_read = read(STDIN_FILENO, buffer, 4095)) > 0)
	{
		buffer[bytes_read] = '\0';
		ft_putstr_fd(buffer, STDOUT_FILENO);
	}
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (0);
}

static t_token	*ft_find_cmd_token(const t_token *token)
{
	t_token	*current;

	current = token->prev;
	while (current && current->type != CMD_EXEC && current->type != CMD_BUILDIN)
		current = current->prev;
	return (current);
}

int	exec_heredoc(t_token *token, t_mini *ms)
{
	int			fd[2];
	t_token		*next;
	t_token		*cmd_token;
	int			heredoc_status;

	if (!token || !token->cmd || ft_strcmp(token->cmd, "<<") != 0)
	{
		ft_putstr_fd("minishell: invalid heredoc token\n", 2);
		return (127);
	}
	next = token->next;
	if (!next || !next->cmd)
	{
		ft_putstr_fd("minishell: no delimiter provided\n", 2);
		return (127);
	}
	cmd_token = ft_find_cmd_token(token);
	if (!cmd_token || !cmd_token->cmd)
	{
		ft_putstr_fd("minishell: no command found\n", 2);
		return (ft_heredoc_default(fd[0]));
	}
	heredoc_status = ft_handle_heredoc_input(next, fd);
	if (heredoc_status != 0)
		return (heredoc_status);
	if (cmd_token->type == CMD_EXEC)
		return (ft_execute_external(cmd_token, ms, fd[0]));
	if (cmd_token->type == CMD_BUILDIN)
		return (ft_execute_builtin(cmd_token, ms, fd[0]));
	ft_putstr_fd("minishell: invalid command type\n", 2);
	return (ft_heredoc_default(fd[0]));
}