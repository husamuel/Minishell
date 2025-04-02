#include "minishell.h"

void add_to_args_file(t_token *token, char *arg)
{
    int i = 0;
    char **new_args;

    while (token->args_file[i])
        i++;

    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;

    ft_memcpy(new_args, token->args_file, sizeof(char *) * i);

    new_args[i] = ft_strdup(arg);
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

	current->args_file[0] = strdup(current->cmd);
		
	if (is_builtin_command(current->cmd) || is_exec_command(current->cmd)) {
		set_command_type(current);
		*command_seen = 1;
		*last_cmd = current;
	}
	else {
			current->type = CMD_NONE;
	}
	
	current->args_file[1] = NULL;
}

static void handle_argument_token(t_token *current, t_token *prev,
	t_token *last_cmd, t_mini *ms)
{
	char *processed_arg = NULL;
	(void)prev;
	(void)ms;
	if (strcmp(current->cmd, "$?") == 0) {
		current->type = CMD_EXIT_STATUS;
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

	if (last_cmd && processed_arg) {
		current->type = CMD_ARG;
		add_to_args_file(last_cmd, processed_arg);
	}
	free(processed_arg);
}

static int is_subshell(t_token *current)
{
	if (current && current->cmd && 
		(ft_strcmp(current->cmd, "./minishell") == 0))
	{
		current->type = CMD_SUBSHELL;
		return (1);
	}
	return (0);
}

void process_token(t_token *current, t_token *prev,
	t_token **last_cmd, int *command_seen, t_mini *ms)
{
    static int in_expr_command = 0;
    
    if (is_subshell(current))
    {
        execute_command(current);
        return;
    }
    
    if (ft_strcmp(current->cmd, "expr") == 0)
    {
        current->type = CMD_EXPR;
        in_expr_command = 1;
        *command_seen = 1;
        *last_cmd = current;
        
        current->args_file = malloc(sizeof(char *) * 2);
        if (!current->args_file)
            return;
        
        current->args_file[0] = strdup(current->cmd);
        current->args_file[1] = NULL;
        return;
    }
    
    if (ft_strcmp(current->cmd, "$?") == 0) {
        current->type = CMD_EXIT_STATUS;
        
        
        if (in_expr_command && *last_cmd) {
            add_to_args_file(*last_cmd, current->cmd);
        }
        
        return;
    }
    
    if (in_expr_command && 
        (ft_strcmp(current->cmd, "+") == 0 || 
         ft_strcmp(current->cmd, "-") == 0 || 
         ft_strcmp(current->cmd, "*") == 0 ||
         ft_strcmp(current->cmd, "/") == 0 || 
         ft_strcmp(current->cmd, "%") == 0)) {
        
        current->type = CMD_PLUS;
        
        if (*last_cmd) {
            add_to_args_file(*last_cmd, current->cmd);
        }
        
        return;
    }
    
    if (current->cmd[0] == '|') {
        current->type = CMD_PIPE;
        ++ms->pipe;
        *command_seen = 0;
        *last_cmd = NULL;
        in_expr_command = 0;
    }
    else if (current->cmd[0] == '\\' || current->cmd[0] == ';') {
        current->type = CMD_NONE;
        in_expr_command = 0;
        return;
    }
    else if (!(*command_seen)) {
        handle_command_token(current, last_cmd, command_seen);
        in_expr_command = 0;
    }
    else {
        if (in_expr_command && *last_cmd) {
            add_to_args_file(*last_cmd, current->cmd);
            current->type = CMD_ARG;
        } else {
            handle_argument_token(current, prev, *last_cmd, ms);
        }
    }
}