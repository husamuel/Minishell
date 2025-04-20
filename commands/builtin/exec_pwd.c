#include "./../../minishell.h"

void	exec_pwd(t_token *token)
{
	char	*current_path;
	char	buffer[BUFFER_SIZE];

	(void)token;
	current_path = getcwd(buffer, BUFFER_SIZE);
	if (!current_path)
	{
		perror("pwd");
		return ;
	}
	printf("%s\n", current_path);
}
