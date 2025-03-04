#include "minishell.h"

void add_to_args_file(t_token *token, char *arg)
{
	int i;
	char **new_args;
	
	for (i = 0; token->args_file[i]; i++)
		;
	
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return;
	
	for (i = 0; token->args_file[i]; i++)
		new_args[i] = token->args_file[i];
	
	new_args[i] = strdup(arg);
	new_args[i + 1] = NULL;
	
	free(token->args_file);
	token->args_file = new_args;
}

static void handle_command_token(t_token *current, t_token **last_cmd,
	int *command_seen)
{
	current->args_file = malloc(sizeof(char *) * 2);
	if (!current->args_file)
		return;

	if (current->cmd[0] == '\'') {
		current->type = CMD_SINGLE_QUOTE;
	}
	else if (current->cmd[0] == '"') {
		current->type = CMD_DOUBLE_QUOTE;
	}
	else {
		current->args_file[0] = strdup(current->cmd);
		
		if (is_builtin_command(current->cmd) || is_exec_command(current->cmd)) {
			set_command_type(current);
			*command_seen = 1;
			*last_cmd = current;
		}
		else {
			current->type = CMD_NONE;
			fprintf(stderr, "%s: command not found\n", current->cmd);
		}
	}
	current->args_file[1] = NULL;
}

static void handle_argument_token(t_token *current, t_token *prev,
	t_token *last_cmd, t_mini *ms)
{
	char *processed_arg = NULL;

	if (strcmp(current->cmd, "$?") == 0) {
		current->type = CMD_EXIT_STATUS;
		char exit_status[16];
		sprintf(exit_status, "%d", ms->exit_status);
		processed_arg = strdup(exit_status);
	}
	else if (current->cmd[0] == '\'') {
		current->type = CMD_SINGLE_QUOTE;
	}
	else if (current->cmd[0] == '"') {
		current->type = CMD_DOUBLE_QUOTE;
	}
	else {
		processed_arg = strdup(current->cmd);
	}

	if (prev && prev->type == CMD_REDIRECT) {
		current->type = CMD_ARG_FILE;
		if (prev && processed_arg)
			add_to_args_file(prev, processed_arg);
	}
	else if (last_cmd && processed_arg) {
		current->type = CMD_ARG;
		add_to_args_file(last_cmd, processed_arg);
	}

	free(processed_arg);
}

static int is_subshell(t_token *current)
{
	if (current && current->cmd && 
		(strcmp(current->cmd, "./minishell") == 0))
	{
		current->type = CMD_SUBSHELL;
		return (1);
	}
	return (0);
}

void process_token(t_token *current, t_token *prev,
	t_token **last_cmd, int *command_seen, t_mini *ms)
{
    if (is_subshell(current))
    {
        *command_seen = 1;
        *last_cmd = current;
        
        if (!current->args_file) {
            current->args_file = malloc(sizeof(char *) * 2);
            if (!current->args_file)
                return;
            current->args_file[0] = strdup(current->cmd);
            current->args_file[1] = NULL;
        }
        
        execute_command(ms, current);
        return;
    }

	if (strcmp(current->cmd, "$?") == 0) {
		printf("%d\n", ms->exit_status);
		current->type = CMD_EXIT_STATUS;
		*command_seen = 0;
		return;
	}
	if (current->cmd[0] == '\\') {
		current->type = CMD_NONE;
		return;
	}
	if (current->cmd[0] == ';') {
		current->type = CMD_NONE;
		return;
	}
	if (current->cmd[0] == '|') {
		current->type = CMD_PIPE;
		*command_seen = 0;
		*last_cmd = NULL;
	}
	else if (current->cmd[0] == '<' || current->cmd[0] == '>') {
		current->type = CMD_REDIRECT;
		if (current->cmd[1] == '<')
			current->type = CMD_HEREDOC;
		*last_cmd = NULL;
	}
	else if (!(*command_seen))
		handle_command_token(current, last_cmd, command_seen);
	else
		handle_argument_token(current, prev, *last_cmd, ms);
}