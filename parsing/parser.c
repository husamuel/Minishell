#include "./../minishell.h"

int	parser(t_mini *ms)
{
	t_token	*current;
	t_token	*prev;
	t_token	*last_cmd;
	int		command_seen;

	if (ms->input == NULL)
		return (0);
	current = ms->token;
	prev = NULL;
	last_cmd = NULL;
	command_seen = 0;
	while (current)
	{
		process_token(current, prev, &last_cmd, &command_seen, ms);
		prev = current;
		current = current->next;
	}
	return (1);
}