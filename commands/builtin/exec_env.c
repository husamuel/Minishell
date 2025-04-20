#include "./../../minishell.h"

void	exec_env(t_token *token, t_mini *mini)
{
	t_env	*ev;
	int		i;

	i = 0;
	ev = mini->export;
	(void)token;
	while (ev)
	{
		if (ev->content)
		{
			printf("%s\n", ev->content);
			i++;
		}
		ev = ev->next;
	}
	printf("\n");
}