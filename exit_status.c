#include "minishell.h"

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int sig)
{
    (void)sig;
}

void setup_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
}

void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void execute_command(t_mini *ms, t_token *cmd)
{
    pid_t pid;
    int status;
    
    if (!cmd || !cmd->cmd || cmd->type == CMD_EXIT_STATUS)
        return;
    
    void (*old_sigint)(int) = signal(SIGINT, SIG_IGN);
    void (*old_sigquit)(int) = signal(SIGQUIT, SIG_IGN);
    
    pid = fork();
    if (pid == 0)
    {
        reset_signals();
        execvp(cmd->cmd, cmd->args_file);
        perror("minishell");
        exit(127);  // Retorna 127 caso execvp falhe
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            // Se o processo filho terminar com sucesso
            ms->exit_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            // Se o processo filho for encerrado por um sinal
            ms->exit_status = 128 + WTERMSIG(status);
        }

        signal(SIGINT, old_sigint);
        signal(SIGQUIT, old_sigquit);
    }
    else
    {
        perror("minishell: fork");
        ms->exit_status = 1;  // Retorna 1 caso o fork falhe
    }
}
