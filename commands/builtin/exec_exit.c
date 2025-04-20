#include "./../../minishell.h"

void	exec_exit(t_token *token)
{
	(void)token;
	printf("exit\n");
	exit(1);
}
