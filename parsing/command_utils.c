#include "./../minishell.h"

static int	is_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (st.st_mode & S_IXUSR)
			return (1);
	}
	return (0);
}

static char	*check_command_in_dir(char *cmd, char *dir)
{
	char	*path;
	char	*tmp;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!path)
		return (NULL);
	if (is_executable(path))
		return (path);
	free(path);
	return (NULL);
}

int	is_exec_command(char *cmd)
{
	char	*path_env;
	char	**dirs;
	char	*cmd_path;
	int		i;

	if (!cmd || cmd[0] == '\0')
		return (0);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (is_executable(cmd));
	path_env = getenv("PATH");
	if (!path_env)
		return (0);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (0);
	i = 0;
	while (dirs[i])
	{
		cmd_path = check_command_in_dir(cmd, dirs[i]);
		if (cmd_path)
		{
			free(cmd_path);
			free_mat(dirs);
			return (1);
		}
		i++;
	}
	free_mat(dirs);
	return (0);
}