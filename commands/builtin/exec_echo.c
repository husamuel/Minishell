#include "./../../minishell.h"

char *print_echo(char *input, int *i, char *var, t_mini *mini);
void echo_others(t_token *next, int i, t_mini *mini, char *input);
void echo_dollar(int *i, char *input, t_mini *mini);
int check_nl_echo(char *s);

void exec_echo(t_token *token, t_mini *mini)
{
	t_token *next;
	char *content;

	next = token->next;
	if (!next)
		printf("\n");
	else if (ft_strcmp(next->cmd, "~") == 0)
	{
		content = getenv("HOME");
		printf("%s\n", content);
	}
	else
		echo_others(next, 0, mini, mini->input + 5);
}

int check_nl_echo(char *s)
{
	int i;

	i = 1;
	while (s[i] == 'n')
		i++;
	if (s[i] && s[i] != ' ')
		return (0);
	return (i);
}

void echo_others(t_token *next, int i, t_mini *mini, char *input)
{
	int j;

	j = 0;
	if (ft_strncmp(next->cmd, "-n", 2) == 0 && check_nl_echo(next->cmd))
	{
		j = check_nl_echo(next->cmd) + 1;
		input += j;
	}
	while (input[i])
	{
		if (input[i] == '\"' || ((input[i -1] != '\"' && input[i +1] != '\"')
			&& input[i] == '\''))
			i++;
		else if (input[i -1] != '\'' && input[i] == '$')
			echo_dollar(&i, input, mini);
		else
		{
			printf("%c", input[i]);
			i++;
		}
	}
	if (j == 0)
		printf("\n");
}

void echo_dollar(int *i, char *input, t_mini *mini)
{
	char *var;
	var = NULL;

	if (input[*i + 1] == '?')
	{
		printf("0");
		*i += 2;
		if (input[*i] == ' ')
			(*i)++;
		return;
	}
	else if (ft_isdigit(input[*i + 1]))
	{
		if (input[*i + 1] == '0')
			printf("minishell");
		*i += 2;
		if (input[*i] == ' ' && input[*i - 1] != '0')
			(*i)++;
	}
	else if (ft_isalpha(input[*i + 1]))
	{
		var = ft_strdup(&input[++(*i)]);
		if (!print_echo(input, i, var, mini) && input[*i] == ' ')
			(*i)++;
	}
}

char *print_echo(char *input, int *i, char *var, t_mini *mini)
{
	char *s;
	int j;

	j = 0;
	while (ft_isalnum(var[j]))
		j++;
	var[j] = '\0';
	s = expand_var(var, mini->export);
	free(var);
	if (s)
	{
		printf("%s", s);
		free(s);
	}
	while (ft_isalnum(input[*i]))
		(*i)++;
	return (s);
}