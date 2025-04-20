#include "./../../minishell.h"

void	swap_var(t_env *current, t_env *next, t_env **head);

void	print_export(t_env	*head)
{
	int	i;

	i = 0;
	while (head)
	{
		i = ft_strlen(head->var) + 1;
		if (!head->content)
			printf("declare -x %s\n", head->var);
		else
			printf("declare -x %s=\"%s\"\n", head->var, head->content + i);
		head = head->next;
	}
}

t_env	*find_node(char *var, t_env *head)
{
	while (head)
	{
		if (ft_strcmp(var, head->var) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

void	append_node(char *var, char *content, t_env *head)
{
	t_env	*node;
	t_env	*last;

	node = malloc(sizeof(t_env));
	if (!node)
		return ;
	node->content = content;
	node->var = var;
	node->next = NULL;
	last = ft_last(head);
	node->prev = last;
	last->next = node;
}

void	order_var(t_mini *mini)
{
	t_env	*head;
	t_env	*current;

	head = mini->export;
	current = head;
	while (current && current->next)
	{
		if (ft_strcmp(current->var, current->next->var) > 0)
		{
			swap_var(current, current->next, &head);
			current = head;
		}
		else
			current = current->next;
	}
	mini->export = head;
}

void	swap_var(t_env *current, t_env *next, t_env **head)
{
	if (*head == current)
		*head = next;
	if (current->prev)
	{
		current->prev->next = next;
		next->prev = current->prev;
	}
	else
		next->prev = NULL;
	if (next->next)
	{
		next->next->prev = current;
		current->next = next->next;
	}
	else
		current->next = NULL;
	next->next = current;
	current->prev = next;
}
