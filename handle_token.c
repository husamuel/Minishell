#include "minishell.h"

void add_to_args(t_token *token, char *arg)
{
    int i = 0;
    char **new_args;
    
    if (token->args) {
        while (token->args[i])
            i++;
    }
    
    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;
    
    if (token->args) {
        for (int j = 0; j < i; j++)
            new_args[j] = token->args[j];
        free(token->args);
    }
    
    new_args[i] = strdup(arg);
    new_args[i + 1] = NULL;
    
    token->args = new_args;
}

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

static void handle_command_token(t_token *current, t_token **last_cmd, int *command_seen)
{
    current->args_file = malloc(sizeof(char *) * 2);
    if (!current->args_file)
        return;
    
    current->args_file[0] = strdup(current->cmd);
    current->args_file[1] = NULL;
    
    current->args = malloc(sizeof(char *) * 2);
    if (!current->args)
        return;
    
    current->args[0] = strdup(current->cmd);
    current->args[1] = NULL;
    
    if (is_builtin_command(current->cmd) || is_exec_command(current->cmd)) {
        set_command_type(current);
        *command_seen = 1;
        *last_cmd = current;
    } else {
        current->type = CMD_NONE;
    }
}

static void handle_argument_token(t_token *current, t_token *prev, t_token *last_cmd, t_mini *ms)
{
    char *processed_arg = NULL;
    (void)prev;
    (void)ms;
    
    if (!last_cmd)
        return;
    
    processed_arg = strdup(current->cmd);
    if (processed_arg)
    {
        add_to_args_file(last_cmd, processed_arg);
        add_to_args(last_cmd, processed_arg);
        current->type = CMD_ARG;
    }
}

static int is_subshell(t_token *current)
{
    if (current && current->cmd && (ft_strcmp(current->cmd, "./minishell") == 0))
    {
        current->type = CMD_SUBSHELL;
        return (1);
    }
    return (0);
}

void process_token(t_token *current, t_token *prev, t_token **last_cmd, int *command_seen, t_mini *ms)
{
    if (prev && prev->cmd && prev->cmd[0] == '|') {
        *command_seen = 0;
        *last_cmd = NULL;
    }
    
    if (is_subshell(current)) {
        execute_command(current);
        return;
    }
    
    if (ft_strcmp(current->cmd, "expr") == 0) {
        current->type = CMD_EXPR;
        *command_seen = 1;
        *last_cmd = current;
        
        current->args_file = malloc(sizeof(char *) * 2);
        if (!current->args_file)
            return;
        current->args_file[0] = strdup(current->cmd);
        current->args_file[1] = NULL;
        
        current->args = malloc(sizeof(char *) * 2);
        if (!current->args)
            return;
        current->args[0] = strdup(current->cmd);
        current->args[1] = NULL;
        
        return;
    }
    
    if (current->cmd[0] == '|') {
        current->type = CMD_PIPE;
        ++ms->pipe;
    }
    else if (current->cmd[0] == '\\' || current->cmd[0] == ';') {
        current->type = CMD_NONE;
        return;
    }
    else if (!(*command_seen)) {
        handle_command_token(current, last_cmd, command_seen);
    }
    else {
        handle_argument_token(current, prev, *last_cmd, ms);
    }
}