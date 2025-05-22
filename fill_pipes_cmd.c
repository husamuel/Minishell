#include "minishell.h"

char	*ft_strjoin_space(const char *s1, const char *s2)
{
	size_t	len1 = ft_strlen(s1);
	size_t	len2 = ft_strlen(s2);
	char	*result;
	size_t	i = 0;
	size_t	j = 0;

	result = malloc(len1 + len2 + 2);
	if (!result)
		return (NULL);

	while (s1 && s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	if (len1 > 0)
		result[i++] = ' ';
	while (s2 && s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}

static int	count_pipe_segments(t_token *token)
{
	int count = 1;
	while (token)
	{
		if (token->type == CMD_PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

static char	*join_command_segment(t_token **token)
{
	char	*segment = NULL;
	char	*tmp;

	while (*token && (*token)->type != CMD_PIPE)
	{
		tmp = segment;
		if (!segment)
			segment = ft_strdup((*token)->cmd);
		else
			segment = ft_strjoin_space(segment, (*token)->cmd);
		free(tmp);
		*token = (*token)->next;
	}
	if (*token && (*token)->type == CMD_PIPE)
		*token = (*token)->next;
	return (segment);
}

static int	*extract_command_types(t_token *token, int count)
{
	int		*types;
	int		i = 0;
	t_token	*start;

	types = malloc(sizeof(int) * count);
	if (!types)
		return (NULL);

	while (token && i < count)
	{
		start = token;
		while (token && token->type != CMD_PIPE)
			token = token->next;
		types[i++] = start->type;
		if (token && token->type == CMD_PIPE)
			token = token->next;
	}
	return (types);
}

void	fill_pipes_cmd(t_token *token)
{
	t_token	*head = token;
	char	**pipes_cmd;
	int		*pipes_type;
	int		i;
	int		count;

	if (!token)
		return;

	count = count_pipe_segments(token);
	pipes_cmd = malloc(sizeof(char *) * (count + 1));
	if (!pipes_cmd)
		return;

	i = 0;
	while (token && i < count)
	{
		pipes_cmd[i] = join_command_segment(&token);
		i++;
	}
	pipes_cmd[i] = NULL;

	token = head;
	pipes_type = extract_command_types(token, count);
	if (!pipes_type)
	{
		free(pipes_cmd);
		return;
	}

	while (head)
	{
		head->pipes_cmd = pipes_cmd;
		head->pipes_type = &pipes_type[0];
		head = head->next;
	}
}